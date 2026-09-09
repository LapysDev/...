# %AppData%/Sublime Text/Packages/Polychrome/polychrome.py
import io, json, os, re
import sublime, sublime_plugin

# ...
BUFFER_CHARACTER_LIMIT    = 67108864 # --> 64M characters
COLOR_SCHEME_LOCK         = threading.RLock()
COLOR_SCHEME_SIGNATURES   = {}
COLORS                    = ("#FF6060", "#FF9F3F", "#FFCF60", "#60CF60", "#60FFFF", "#6090FF", "#C060FF", "#FF60C0")
DELIMITER_LIMIT           = 262144
DELIMITERS                = (("(", ")"), ("[", "]"), ("{", "}"), ("᚛", "᚜"), ("‹", "›"), ("«", "»"), ("❨", "❩"), ("❪", "❫"), ("❬", "❭"), ("❮", "❯"), ("❰", "❱"), ("❲", "❳"), ("❴", "❵"), ("⟅", "⟆"), ("⟦", "⟧"), ("⟨", "⟩"), ("⟪", "⟫"), ("⟬", "⟭"), ("⟮", "⟯"), ("⦃", "⦄"), ("⦅", "⦆"), ("⦗", "⦘"), ("⧘", "⧙"), ("⧚", "⧛"), ("⸨", "⸩"), ("〈", "〉"), ("《", "》"), ("「", "」"), ("『", "』"), ("【", "】"), ("〔", "〕"), ("〖", "〗"), ("〘", "〙"), ("〚", "〛"), ("﴾", "﴿"), ("﹙", "﹚"), ("﹛", "﹜"), ("﹝", "﹞"), ("（", "）"), ("［", "］"), ("｛", "｝"), ("｟", "｠"), ("｢", "｣"))
DELIMITER_MAP             = dict((closing, opening) for opening, closing in DELIMITERS)
DELIMITER_PATTERN         = re.compile("[" + re.escape("".join(opening + closing for opening, closing in DELIMITERS)) + "]")
DELIMITERS_PER_SCAN_SLICE = 1024
ERASE_KEYS_PER_SLICE      = 4
HAS_TEXT_CHANGE_LISTENER  = hasattr(sublime_plugin, "TextChangeListener")
IGNORED_SCOPE_SELECTOR    = "comment, constant, string"
NESTING_DEPTH_LIMIT       = 65536
PACKAGE_NAME              = "Polychrome"
PACKAGE_VERSION           = "2.0.3"
PAINT_BATCH_SIZE          = 2048
POLYCHROME_SCOPES         = tuple("polychrome.depth." + str(depth) for depth in range(len(COLORS)))
REGION_FLAGS              = sublime.DRAW_NO_OUTLINE | getattr(sublime, "HIDE_ON_MINIMAP", 0)
REGION_SCOPES             = ("region.redish", "region.orangish", "region.yellowish", "region.greenish", "region.cyanish", "region.bluish", "region.purplish", "region.pinkish")
REPAINT_DELAY             = 60
SCAN_CHUNK_SIZE           = 16384 # --> 16K characters
STATE_DICTIONARY_LOCK     = threading.RLock()
STATES                    = {}

# ...
class PaintJob(object):
  __slots__ = ("batch_index", "change_count", "commanded", "depth", "erase_index", "generation", "new_region_keys", "region_index", "regions_by_depth", "stale_region_keys", "state", "syntax", "use_overlay_scopes", "view")

  def __init__(self, scan_job, use_overlay_scopes):
    self.batch_index        = 0
    self.change_count       = scan_job.change_count
    self.commanded          = scan_job.commanded
    self.depth              = 0
    self.erase_index        = 0
    self.generation         = scan_job.generation
    self.new_region_keys    = set()
    self.region_index       = 0
    self.regions_by_depth   = scan_job.regions_by_depth
    self.stale_region_keys  = None
    self.state              = scan_job.state
    self.syntax             = scan_job.syntax
    self.use_overlay_scopes = use_overlay_scopes
    self.view               = scan_job.view

class PolychromeEventListener(sublime_plugin.EventListener):
  def on_activated_async(self, view):
    if not _is_eligible_view(view):
      return

    state = _get_state(view)
    if state is None:
      _schedule_repaint(view, 0)
      return

    with state.lock:
      repaint_needed = state.change_count != view.change_count() or state.syntax != view.settings().get("syntax")

    if repaint_needed:
      _schedule_repaint(view, 0)
      return

    _ensure_color_scheme_overlay(view)

  def on_clone_async(self, view):
    _schedule_repaint(view, 0)

  def on_close(self, view):
    with STATE_DICTIONARY_LOCK:
      STATES.pop(view.id(), None)

  def on_load_async(self, view):
    _schedule_repaint(view, 0)

  def on_modified_async(self, view):
    if not HAS_TEXT_CHANGE_LISTENER:
      _schedule_repaint(view, REPAINT_DELAY)

  def on_new_async(self, view):
    _schedule_repaint(view, 0)

  def on_post_save_async(self, view):
    _schedule_repaint(view, REPAINT_DELAY)

  def on_reload_async(self, view):
    _reset_repaint(view, False, REPAINT_DELAY)

  def on_revert_async(self, view):
    _reset_repaint(view, False, REPAINT_DELAY)

class PolychromeRepaintCommand(sublime_plugin.TextCommand):
  def run(self, edit):
    sublime.status_message("Polychrome: Repainting from scratch...")
    _reset_repaint(self.view)

  def is_enabled(self):
    return _is_eligible_view(self.view)

class ScanJob(object):
  __slots__ = ("change_count", "check_scopes", "commanded", "delimiter_count", "generation", "position", "regions_by_depth", "size", "stack", "state", "syntax", "view")

  def __init__(self, view, state, generation, commanded):
    self.change_count     = view.change_count()
    self.check_scopes     = _syntax_uses_scope_exclusions(view.settings().get("syntax"))
    self.commanded        = commanded
    self.delimiter_count  = 0
    self.generation       = generation
    self.position         = 0
    self.regions_by_depth = [[] for _ in COLORS]
    self.size             = view.size()
    self.stack            = []
    self.state            = state
    self.syntax           = view.settings().get("syntax")
    self.view             = view

class ViewState(object):
  __slots__ = ("active_region_keys", "change_count", "generation", "key_token", "lock", "size", "syntax")

  def __init__(self):
    self.active_region_keys = set()
    self.change_count       = -1
    self.generation         = 0
    self.key_token          = uuid.uuid4().hex
    self.lock               = threading.RLock()
    self.size               = 0
    self.syntax             = None

# ...
def _abort_scan(scan_job, reason):
  print("Polychrome: Repaint aborted - " + reason)

  if scan_job.commanded:
    sublime.status_message("Polychrome: Repaint aborted - " + reason)

  _clear_current_regions(scan_job.view, scan_job.state, scan_job.generation, scan_job.change_count, scan_job.syntax)

def _clear_current_regions(view, state, generation, change_count, syntax, erase_index=0, region_keys=None):
  if not _generation_is_current(view, state, generation):
    return

  if region_keys is None:
    with state.lock:
      region_keys = sorted(state.active_region_keys)

  end_index = min(len(region_keys), erase_index + ERASE_KEYS_PER_SLICE)

  try:
    for region_key in region_keys[erase_index:end_index]:
      view.erase_regions(region_key)

    with state.lock:
      state.active_region_keys.difference_update(region_keys[erase_index:end_index])
      _store_region_keys(view, state.active_region_keys)
  except Exception as error:
    print("Polychrome: Region cleanup failed - " + str(error))
    return

  if end_index < len(region_keys):
    sublime.set_timeout_async(lambda: _clear_current_regions(view, state, generation, change_count, syntax, end_index, region_keys), 0)
    return

  if not _generation_is_current(view, state, generation):
    return

  if view.change_count() == change_count and view.settings().get("syntax") == syntax:
    with state.lock:
      state.change_count = change_count
      state.size         = view.size()
      state.syntax       = syntax

def _discard_paint_job_regions(paint_job, erase_index=0, region_keys=None):
  if paint_job.view is None or not paint_job.view.is_valid():
    return

  if region_keys is None:
    region_keys = sorted(paint_job.new_region_keys)

  end_index = min(len(region_keys), erase_index + ERASE_KEYS_PER_SLICE)

  try:
    for region_key in region_keys[erase_index:end_index]:
      paint_job.view.erase_regions(region_key)
  except Exception as error:
    print("Polychrome: Canceled paint cleanup failed - " + str(error))
    return

  with paint_job.state.lock:
    paint_job.state.active_region_keys.difference_update(region_keys[erase_index:end_index])
    _store_region_keys(paint_job.view, paint_job.state.active_region_keys)

  if end_index < len(region_keys):
    sublime.set_timeout_async(lambda: _discard_paint_job_regions(paint_job, end_index, region_keys), 0)

def _ensure_color_scheme_overlay(view):
  if int(sublime.version()) < 3150:
    return False, False

  color_scheme_resource = view.settings().get("color_scheme")
  if not color_scheme_resource:
    return False, False

  background = "#000000"
  filename   = os.path.splitext(color_scheme_resource.replace("\\", "/").rsplit("/", 1)[-1])[0] + ".sublime-color-scheme"

  try:
    background = view.style().get("background", background)
  except Exception:
    return False, False

  if not isinstance(background, str):
    nearest_background = "#000001"
  elif len(background) == 7 and background.startswith("#"):
    try:
      color_value = int(background[1:], 16)
    except ValueError:
      nearest_background = background
    else:
      blue               = (color_value >> 0)  & 0xFF
      green              = (color_value >> 8)  & 0xFF
      red                = (color_value >> 16) & 0xFF
      nearest_background = "#{:02X}{:02X}{:02X}".format(red + 1 if red < 255 else red - 1, green, blue)
  else:
    nearest_background = background

  signature    = (nearest_background,) + COLORS
  overlay_path = os.path.join(sublime.packages_path(), "User", "Color Schemes", PACKAGE_NAME, filename)

  with COLOR_SCHEME_LOCK:
    if COLOR_SCHEME_SIGNATURES.get(overlay_path) == signature and os.path.isfile(overlay_path):
      return True, False

    rules = []
    for depth, foreground in enumerate(COLORS):
      rules.append({"background": nearest_background, "foreground": foreground, "name": "Polychrome depth " + str(depth + 1), "scope": POLYCHROME_SCOPES[depth]})

    overlay_text  = json.dumps({"name": "Polychrome generated overlay", "rules": rules}, indent=2, sort_keys=True) + "\n"
    wrote_overlay = _write_text_if_changed(overlay_path, overlay_text)

    if not wrote_overlay:
      try:
        with io.open(overlay_path, "r", encoding="utf-8") as overlay_file:
          if overlay_file.read() != overlay_text:
            return False, False
      except (IOError, OSError):
        return False, False

    COLOR_SCHEME_SIGNATURES[overlay_path] = signature
    return True, wrote_overlay

def _finish_paint(paint_job):
  if not _is_eligible_view(paint_job.view):
    _discard_paint_job_regions(paint_job)
    return

  with STATE_DICTIONARY_LOCK:
    if STATES.get(paint_job.view.id()) is not paint_job.state:
      paint_is_current = False
    else:
      with paint_job.state.lock:
        paint_is_current = paint_job.state.generation == paint_job.generation and paint_job.view.change_count() == paint_job.change_count and paint_job.view.settings().get("syntax") == paint_job.syntax

        if paint_is_current:
          # ->> Every live key was already registered as it was painted, while stale keys were removed during cleanup. Do not replace this set here: doing so after a concurrent reset could resurrect detached-state keys.
          paint_job.state.change_count = paint_job.change_count
          paint_job.state.size         = paint_job.view.size()
          paint_job.state.syntax       = paint_job.syntax

  if not paint_is_current:
    _discard_paint_job_regions(paint_job)
    return

  if paint_job.commanded:
    sublime.status_message("Polychrome: Repaint complete")

def _generation_is_current(view, state, generation):
  if not _is_eligible_view(view):
    return False

  with STATE_DICTIONARY_LOCK:
    if STATES.get(view.id()) is not state:
      return False

  with state.lock:
    return state.generation == generation

def _get_state(view, create=False):
  with STATE_DICTIONARY_LOCK:
    state = STATES.get(view.id())

    if state is None and create:
      state             = ViewState()
      STATES[view.id()] = state

    return state

def _is_eligible_view(view):
  return view is not None and view.is_valid() and not view.is_loading() and not view.settings().get("is_widget", False)

def _job_is_current(job):
  if not _generation_is_current(job.view, job.state, job.generation):
    return False

  return job.view.change_count() == job.change_count and job.view.settings().get("syntax") == job.syntax

def _paint_continue(paint_job):
  if not _job_is_current(paint_job):
    _discard_paint_job_regions(paint_job)
    return

  while paint_job.depth < len(COLORS) and paint_job.region_index >= len(paint_job.regions_by_depth[paint_job.depth]):
    paint_job.depth       += 1
    paint_job.batch_index  = 0
    paint_job.region_index = 0

  if paint_job.depth >= len(COLORS):
    _start_stale_region_cleanup(paint_job)
    return

  points     = paint_job.regions_by_depth[paint_job.depth]
  end_index  = min(len(points), paint_job.region_index + PAINT_BATCH_SIZE)
  region_key = _region_key(paint_job.state, paint_job.generation, paint_job.depth, paint_job.batch_index)
  scopes     = POLYCHROME_SCOPES if paint_job.use_overlay_scopes else REGION_SCOPES

  try:
    # ->> Make the final validity check, region write and key registration atomic with respect to scheduling/resetting this view. This closes the add_regions() -> state-registration race.
    with STATE_DICTIONARY_LOCK:
      if STATES.get(paint_job.view.id()) is not paint_job.state:
        paint_is_current = False
      else:
        with paint_job.state.lock:
          paint_is_current = paint_job.state.generation == paint_job.generation and paint_job.view.change_count() == paint_job.change_count and paint_job.view.settings().get("syntax") == paint_job.syntax

          if paint_is_current:
            paint_job.view.add_regions(region_key, [sublime.Region(point, point + 1) for point in points[paint_job.region_index:end_index]], scopes[paint_job.depth], "", REGION_FLAGS)
            paint_job.new_region_keys.add(region_key)
            paint_job.state.active_region_keys.add(region_key)
            _store_region_keys(paint_job.view, paint_job.state.active_region_keys)
  except Exception as error:
    print("Polychrome: Painting failed - " + str(error))
    _clear_current_regions(paint_job.view, paint_job.state, paint_job.generation, paint_job.change_count, paint_job.syntax)
    return

  if not paint_is_current:
    _discard_paint_job_regions(paint_job)
    return

  paint_job.batch_index  += 1
  paint_job.region_index  = end_index
  sublime.set_timeout_async(lambda: _paint_continue(paint_job), 0)

def _region_key(state, generation, depth, batch_index):
  # ->> Generations intentionally reset to zero after a commanded reset, so a per-state token prevents canceled callbacks from an old state erasing or overwriting generation-zero regions in the new state.
  return "polychrome.state." + state.key_token + ".generation." + str(generation) + ".depth." + str(depth) + ".batch." + str(batch_index)

def _reset_repaint(view, commanded=True, delay=0):
  if not _is_eligible_view(view):
    return

  view_id = view.id()

  with STATE_DICTIONARY_LOCK:
    old_state   = STATES.get(view_id)
    region_keys = set(view.settings().get("polychrome.region_keys", []))

    if old_state is not None:
      with old_state.lock:
        # ->> Invalidate every queued/running job before replacing its state.
        old_state.generation += 1
        region_keys.update(old_state.active_region_keys)
        old_state.active_region_keys.clear()

    region_keys     = sorted(region_keys)
    state           = ViewState()
    STATES[view_id] = state

  # ->> Keep the complete cleanup manifest until each key has actually been erased.
  # This lets a later commanded repaint recover even if a reset is interrupted.
  _store_region_keys(view, region_keys)
  sublime.set_timeout_async(lambda: _reset_region_cleanup_continue(view, state, region_keys, commanded, delay), 0)

def _reset_region_cleanup_continue(view, state, region_keys, commanded, delay, erase_index=0):
  if not _is_eligible_view(view):
    return

  end_index = min(len(region_keys), erase_index + ERASE_KEYS_PER_SLICE)

  try:
    for region_key in region_keys[erase_index:end_index]:
      view.erase_regions(region_key)
  except Exception as error:
    print("Polychrome: Reset cleanup failed - " + str(error))
    sublime.status_message("Polychrome: Repaint aborted - reset cleanup failed")
    return

  _store_region_keys(view, region_keys[end_index:])

  if end_index < len(region_keys):
    sublime.set_timeout_async(lambda: _reset_region_cleanup_continue(view, state, region_keys, commanded, delay, end_index), 0)
    return

  # ->> Do not scan until every previous Polychrome region key has been erased.
  # A reset starts from generation zero; any intervening repaint request or newer reset supersedes it.
  with STATE_DICTIONARY_LOCK:
    if STATES.get(view.id()) is not state:
      return

  with state.lock:
    if state.generation != 0:
      return

  sublime.set_timeout_async(lambda: _start_repaint(view, state, 0, commanded), delay)

def _scan_continue(scan_job):
  if not _job_is_current(scan_job):
    return

  if scan_job.size > BUFFER_CHARACTER_LIMIT:
    _abort_scan(scan_job, "buffer character limit exceeded")
    return

  if scan_job.position >= scan_job.size:
    _start_paint(scan_job)
    return

  end = min(scan_job.size, scan_job.position + SCAN_CHUNK_SIZE)

  try:
    text = scan_job.view.substr(sublime.Region(scan_job.position, end))
  except Exception as error:
    print("Polychrome: Buffer read failed - " + str(error))
    return

  if not _job_is_current(scan_job):
    return

  delimiters_this_slice = 0

  try:
    for match in DELIMITER_PATTERN.finditer(text):
      point = scan_job.position + match.start()
      scan_job.delimiter_count += 1
      delimiters_this_slice    += 1

      if scan_job.delimiter_count > DELIMITER_LIMIT:
        _abort_scan(scan_job, "delimiter limit exceeded")
        return

      if scan_job.check_scopes and scan_job.view.match_selector(point, IGNORED_SCOPE_SELECTOR):
        if delimiters_this_slice >= DELIMITERS_PER_SCAN_SLICE:
          scan_job.position = point + 1
          sublime.set_timeout_async(lambda: _scan_continue(scan_job), 0)
          return
        continue

      delimiter         = match.group(0)
      opening_delimiter = DELIMITER_MAP.get(delimiter)

      if opening_delimiter is None:
        if len(scan_job.stack) >= NESTING_DEPTH_LIMIT:
          _abort_scan(scan_job, "nesting depth limit exceeded")
          return

        scan_job.regions_by_depth[len(scan_job.stack) % len(COLORS)].append(point)
        scan_job.stack.append(delimiter)
      elif scan_job.stack and opening_delimiter == scan_job.stack[-1]:
        scan_job.regions_by_depth[(len(scan_job.stack) - 1) % len(COLORS)].append(point)
        scan_job.stack.pop()

      if delimiters_this_slice >= DELIMITERS_PER_SCAN_SLICE:
        scan_job.position = point + 1
        sublime.set_timeout_async(lambda: _scan_continue(scan_job), 0)
        return
  except Exception as error:
    print("Polychrome: Scanning failed - " + str(error))
    _abort_scan(scan_job, "scanner error")
    return

  scan_job.position = end
  sublime.set_timeout_async(lambda: _scan_continue(scan_job), 0)

def _schedule_repaint(view, delay=REPAINT_DELAY, commanded=False):
  if not _is_eligible_view(view):
    return

  state = _get_state(view, True)

  with state.lock:
    state.generation += 1
    generation        = state.generation

  sublime.set_timeout_async(lambda: _start_repaint(view, state, generation, commanded), delay)

def _store_region_keys(view, region_keys):
  region_keys = sorted(region_keys)

  if region_keys:
    view.settings().set("polychrome.region_keys", region_keys)
  else:
    view.settings().erase("polychrome.region_keys")

def _stale_region_cleanup_continue(paint_job):
  if not _job_is_current(paint_job):
    _discard_paint_job_regions(paint_job)
    return

  end_index = min(len(paint_job.stale_region_keys), paint_job.erase_index + ERASE_KEYS_PER_SLICE)

  try:
    for region_key in paint_job.stale_region_keys[paint_job.erase_index:end_index]:
      paint_job.view.erase_regions(region_key)

    with paint_job.state.lock:
      paint_job.state.active_region_keys.difference_update(paint_job.stale_region_keys[paint_job.erase_index:end_index])
      _store_region_keys(paint_job.view, paint_job.state.active_region_keys)
      generation_is_current = paint_job.state.generation == paint_job.generation

    if not generation_is_current:
      _discard_paint_job_regions(paint_job)
      return
  except Exception as error:
    print("Polychrome: Stale region cleanup failed - " + str(error))
    _clear_current_regions(paint_job.view, paint_job.state, paint_job.generation, paint_job.change_count, paint_job.syntax)
    return

  paint_job.erase_index = end_index

  if paint_job.erase_index < len(paint_job.stale_region_keys):
    sublime.set_timeout_async(lambda: _stale_region_cleanup_continue(paint_job), 0)
    return

  _finish_paint(paint_job)

def _start_paint(scan_job):
  if not _job_is_current(scan_job):
    return

  use_overlay_scopes, wrote_overlay = _ensure_color_scheme_overlay(scan_job.view)
  paint_job                         = PaintJob(scan_job, use_overlay_scopes)

  if wrote_overlay:
    sublime.set_timeout_async(lambda: _paint_continue(paint_job), 120)
  else:
    sublime.set_timeout_async(lambda: _paint_continue(paint_job), 0)

def _start_repaint(view, state, generation, commanded):
  if not _generation_is_current(view, state, generation):
    return

  scan_job = ScanJob(view, state, generation, commanded)
  sublime.set_timeout_async(lambda: _scan_continue(scan_job), 0)

def _start_stale_region_cleanup(paint_job):
  if not _job_is_current(paint_job):
    _discard_paint_job_regions(paint_job)
    return

  with paint_job.state.lock:
    paint_job.stale_region_keys = sorted(paint_job.state.active_region_keys - paint_job.new_region_keys)

  if not paint_job.stale_region_keys:
    _finish_paint(paint_job)
    return

  sublime.set_timeout_async(lambda: _stale_region_cleanup_continue(paint_job), 0)

def _changes_replace_entire_buffer(view, changes):
  if len(changes) != 1:
    return False

  change    = changes[0]
  old_end   = max(change.a.pt, change.b.pt)
  old_start = min(change.a.pt, change.b.pt)
  old_size  = view.size() - (len(change.str) - (old_end - old_start))
  return old_size > 0 and old_start == 0 and old_end >= old_size

def _syntax_uses_scope_exclusions(syntax):
  if not syntax:
    return False

  filename = syntax.replace("\\", "/").rsplit("/", 1)[-1].lower()
  return filename not in ("plain text.sublime-syntax", "plain text.tmlanguage")

def _write_text_if_changed(path, text):
  directory     = os.path.dirname(path)
  previous_text = None

  try:
    with io.open(path, "r", encoding="utf-8") as source_file:
      previous_text = source_file.read()
  except (IOError, OSError):
    pass

  if previous_text == text:
    return False

  try:
    if directory and not os.path.isdir(directory):
      os.makedirs(directory)
  except OSError:
    if not os.path.isdir(directory):
      return False

  try:
    with io.open(path, "w", encoding="utf-8") as destination_file:
      destination_file.write(text)
    return True
  except (IOError, OSError):
    return False

def plugin_loaded():
  package_directory = os.path.join(sublime.packages_path(), PACKAGE_NAME)

  _write_text_if_changed(
    os.path.join(package_directory, "Polychrome.sublime-commands"),
    json.dumps([
      {"caption": "Polychrome: Repaint", "command": "polychrome_repaint"}
    ], indent=2, sort_keys=True) + "\n"
  )

  for window in sublime.windows():
    for view in window.views():
      _reset_repaint(view, False)

def plugin_unloaded():
  with COLOR_SCHEME_LOCK:
    COLOR_SCHEME_SIGNATURES.clear()

  with STATE_DICTIONARY_LOCK:
    states = dict(STATES)
    STATES.clear()

  for window in sublime.windows():
    for view in window.views():
      region_keys = set(view.settings().get("polychrome.region_keys", []))
      state       = states.get(view.id())

      if state is not None:
        with state.lock:
          region_keys.update(state.active_region_keys)
          state.active_region_keys.clear()

      for region_key in region_keys:
        view.erase_regions(region_key)

      _store_region_keys(view, [])

# ...
if HAS_TEXT_CHANGE_LISTENER:
  class PolychromeTextChangeListener(sublime_plugin.TextChangeListener):
    @classmethod
    def is_applicable(cls, buffer):
      view = buffer.primary_view()
      return view is not None and not view.settings().get("is_widget", False)

    def on_text_changed(self, changes):
      for view in self.buffer.views():
        # ->> Added regions track edits, so replacing the complete buffer can drag the old highlights onto unrelated text. Treat that transaction as a state reset instead of leaving transformed regions visible until ordinary stale-key cleanup.
        if _changes_replace_entire_buffer(view, changes):
          _reset_repaint(view, False, REPAINT_DELAY)
        else:
          _schedule_repaint(view, REPAINT_DELAY)
