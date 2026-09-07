# %AppData%/Sublime Text/Packages/Polychrome
import io
import json
import os
import re
import threading

import sublime
import sublime_plugin

# ...
CHECKPOINT_INTERVAL         = 16384 # --> 16KB
COLORS                      = ("#FF6060", "#FF9F43", "#FFD75F", "#5FD75F", "#5FFFFF", "#5F87FF", "#AF5FFF", "#FF5FAF")
COLOR_SCHEME_LOCK           = threading.RLock()
COLOR_SCHEME_SIGNATURES     = {}
DELIMITERS                  = (("(", ")"), ("[", "]"), ("{", "}"), ("᚛", "᚜"), ("‹", "›"), ("«", "»"), ("❨", "❩"), ("❪", "❫"), ("❬", "❭"), ("❮", "❯"), ("❰", "❱"), ("❲", "❳"), ("❴", "❵"), ("⟅", "⟆"), ("⟦", "⟧"), ("⟨", "⟩"), ("⟪", "⟫"), ("⟬", "⟭"), ("⟮", "⟯"), ("⦃", "⦄"), ("⦅", "⦆"), ("⦗", "⦘"), ("⧘", "⧙"), ("⧚", "⧛"), ("⸨", "⸩"), ("〈", "〉"), ("《", "》"), ("「", "」"), ("『", "』"), ("【", "】"), ("〔", "〕"), ("〖", "〗"), ("〘", "〙"), ("〚", "〛"), ("﴾", "﴿"), ("﹙", "﹚"), ("﹛", "﹜"), ("﹝", "﹞"), ("（", "）"), ("［", "］"), ("｛", "｝"), ("｟", "｠"), ("｢", "｣"))
DELIMITER_MAP               = dict((closing, opening) for opening, closing in DELIMITERS)
DELIMITER_PATTERN           = re.compile("[" + re.escape("".join(opening + closing for opening, closing in DELIMITERS)) + "]")
HAS_TEXT_CHANGE_LISTENER    = hasattr(sublime_plugin, "TextChangeListener")
IGNORED_SCOPE_SELECTOR      = "comment, constant, string"
MAX_FULL_REPAINT_SEGMENTS   = 65536 # --> 1GB at 16KB checkpoints
MAX_IGNORED_REGION_ADVANCES = 65536
MAX_INCREMENTAL_SCAN        = 262144 # --> 256KB
MAX_INCREMENTAL_SEGMENTS    = 64
MAX_REPAINT_RETRIES         = 3
MAX_SEGMENT_SEARCH          = 65536
PACKAGE_NAME                = "Polychrome"
PACKAGE_VERSION             = "1.2.0"
PENDING_REPAINTS            = {}
POLYCHROME_SCOPES           = tuple("polychrome.depth." + str(depth) for depth in range(len(COLORS)))
REGION_FLAGS                = sublime.DRAW_NO_OUTLINE | getattr(sublime, "HIDE_ON_MINIMAP", 0)
REGION_SCOPES               = ("region.redish", "region.orangish", "region.yellowish", "region.greenish", "region.cyanish", "region.bluish", "region.purplish", "region.pinkish")
STATE_DICTIONARY_LOCK       = threading.RLock()
STATES                      = {}

# ...
class PolychromeEventListener(sublime_plugin.EventListener):
  def on_activated_async(self, view):
    if not _is_eligible_view(view):
      return

    state = _get_state(view)
    if state is None:
      _schedule_full_repaint(view, 0)
      return

    with state.lock:
      if state.change_count != view.change_count() or state.syntax != view.settings().get("syntax"):
        _schedule_full_repaint(view, 0)
        return

    _, wrote_overlay = _ensure_color_scheme_overlay(view)
    if wrote_overlay:
      sublime.set_timeout_async(lambda: _reapply_existing_segments(view), 120)

  def on_clone_async(self, view):
    _schedule_full_repaint(view, 0)

  def on_close(self, view):
    with STATE_DICTIONARY_LOCK:
      PENDING_REPAINTS.pop(view.id(), None)
      STATES.pop(view.id(), None)

  def on_load_async(self, view):
    _schedule_full_repaint(view, 0)

  def on_modified_async(self, view):
    if not HAS_TEXT_CHANGE_LISTENER:
      _schedule_full_repaint(view, 60)

  def on_new_async(self, view):
    _schedule_full_repaint(view, 0)

  def on_reload_async(self, view):
    _schedule_full_repaint(view, 0)

  def on_revert_async(self, view):
    _schedule_full_repaint(view, 0)

class PolychromeRepaintCommand(sublime_plugin.TextCommand):
  def run(self, edit):
    sublime.status_message("Polychrome: Repainting from scratch...")
    _schedule_full_repaint(self.view, 0, True)

  def is_enabled(self):
    return _is_eligible_view(self.view)

class Segment(object):
  __slots__ = ("end", "end_fingerprint", "end_stack", "key_id", "regions_by_depth", "start", "start_stack")

  def __init__(self, key_id, start, end, start_stack, end_stack, end_fingerprint, regions_by_depth):
    self.end              = end
    self.end_fingerprint  = end_fingerprint
    self.end_stack        = end_stack
    self.key_id           = key_id
    self.regions_by_depth = regions_by_depth
    self.start            = start
    self.start_stack      = start_stack

class ViewState(object):
  __slots__ = ("change_count", "lock", "next_segment_id", "segments", "size", "syntax")

  def __init__(self):
    self.change_count    = -1
    self.lock            = threading.RLock()
    self.next_segment_id = 0
    self.segments        = []
    self.size            = 0
    self.syntax          = None

# ...
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
      blue               = color_value & 0xFF
      green              = color_value >> 8 & 0xFF
      red                = color_value >> 16 & 0xFF
      nearest_background = "#" + hex(red + 1 if red < 255 else red - 1)[2:].zfill(2) + hex(green)[2:].zfill(2) + hex(blue)[2:].zfill(2)
  else:
    nearest_background = background

  signature    = (nearest_background,) + COLORS
  overlay_path = os.path.join(sublime.packages_path(), "User", "Color Schemes", PACKAGE_NAME, filename)

  with COLOR_SCHEME_LOCK:
    if COLOR_SCHEME_SIGNATURES.get(filename) == signature and os.path.isfile(overlay_path):
      return True, False

    rules = []
    for depth, foreground in enumerate(COLORS):
      rules.append({
        "background": nearest_background,
        "foreground": foreground,
        "name"      : "Polychrome depth " + str(depth + 1),
        "scope"     : POLYCHROME_SCOPES[depth]
      })

    overlay_text  = json.dumps({"name": "Polychrome generated overlay", "rules": rules}, indent=2, sort_keys=True) + "\n"
    wrote_overlay = _write_text_if_changed(overlay_path, overlay_text)

    if not wrote_overlay:
      try:
        with io.open(overlay_path, "r", encoding="utf-8") as overlay_file:
          if overlay_file.read() != overlay_text:
            return False, False
      except (IOError, OSError):
        return False, False

    COLOR_SCHEME_SIGNATURES[filename] = signature
    return True, wrote_overlay

def _erase_segment(view, segment):
  for depth in range(len(COLORS)):
    view.erase_regions(_segment_key(segment, depth))

def _full_repaint(view, commanded=False, repaint_retry_count=0):
  if not _is_eligible_view(view):
    return

  state = _get_state(view, True)

  with state.lock:
    ignored_region_index = 0
    ignored_regions      = []
    initial_change_count = view.change_count()
    new_segments         = []
    next_segment_id      = 0
    size                 = view.size()
    stack                = ""
    start                = 0

    # ->> Full rebuilds can batch scope exclusion in native code. Incremental scans instead query scopes only for delimiters in the re-parsed checkpoints.
    for ignored_region in sorted(view.find_by_selector(IGNORED_SCOPE_SELECTOR), key=lambda region: region.begin()):
      ignored_end   = ignored_region.end()
      ignored_start = ignored_region.begin()

      if ignored_regions and ignored_start <= ignored_regions[-1][1]:
        if ignored_end > ignored_regions[-1][1]:
          ignored_regions[-1][1] = ignored_end
      else:
        ignored_regions.append([ignored_start, ignored_end])

    full_repaint_segment_count = 0
    while start < size:
      if full_repaint_segment_count >= MAX_FULL_REPAINT_SEGMENTS:
        print("Polychrome: Full repaint aborted - segment limit exceeded")
        if commanded:
          sublime.status_message("Polychrome: Repaint aborted - segment limit exceeded")
        return

      end                                               = min(size, start + CHECKPOINT_INTERVAL)
      end_stack, regions_by_depth, ignored_region_index = _scan_segment(view, start, end, stack, ignored_regions, ignored_region_index)

      new_segments.append(Segment(next_segment_id, start, end, stack, end_stack, _scope_fingerprint(view, end), regions_by_depth))
      full_repaint_segment_count += 1
      next_segment_id           += 1
      stack                      = end_stack
      start                      = end

    # ->> EOF sentinel: an insertion at EOF starts re-parsing at EOF instead of one complete checkpoint earlier.
    new_segments.append(Segment(next_segment_id, size, size, stack, stack, _scope_fingerprint(view, size), [[] for _ in COLORS]))
    next_segment_id += 1

    if view.change_count() != initial_change_count:
      if repaint_retry_count < MAX_REPAINT_RETRIES:
        _schedule_full_repaint(view, 0, commanded, repaint_retry_count + 1)
      else:
        print("Polychrome: Full repaint abandoned - buffer kept changing")
        if commanded:
          sublime.status_message("Polychrome: Repaint abandoned - buffer kept changing")
      return

    use_overlay_scopes, wrote_overlay = _ensure_color_scheme_overlay(view)

    for segment in state.segments:
      _erase_segment(view, segment)

    for segment in new_segments:
      _paint_segment(view, segment, use_overlay_scopes)

    state.change_count    = initial_change_count
    state.next_segment_id = next_segment_id
    state.segments        = new_segments
    state.size            = size
    state.syntax          = view.settings().get("syntax")

  if wrote_overlay:
    sublime.set_timeout_async(lambda: _reapply_existing_segments(view), 120)

  if commanded:
    sublime.status_message("Polychrome: Repaint complete")

def _get_state(view, create=False):
  with STATE_DICTIONARY_LOCK:
    state = STATES.get(view.id())

    if state is None and create:
      state             = ViewState()
      STATES[view.id()] = state

    return state

def _incremental_repaint(view, changes):
  if not _is_eligible_view(view) or not changes:
    return

  state = _get_state(view)
  if state is None:
    _schedule_full_repaint(view, 0)
    return

  with state.lock:
    current_change_count = view.change_count()
    old_segments         = state.segments

    if state.change_count == current_change_count:
      return

    if state.change_count + 1 != current_change_count or state.syntax != view.settings().get("syntax") or not old_segments:
      _schedule_full_repaint(view, 0)
      return

    ordered_changes    = sorted(changes, key=lambda change: change.a.pt)
    earliest_old_start = min(ordered_changes[0].a.pt, ordered_changes[0].b.pt)
    latest_old_end     = max(ordered_changes[0].a.pt, ordered_changes[0].b.pt)
    previous_old_end   = -1
    total_delta        = 0

    for change in ordered_changes:
      old_end   = max(change.a.pt, change.b.pt)
      old_start = min(change.a.pt, change.b.pt)

      # ->> `TextChange` regions from one transaction must not overlap.
      if old_start < previous_old_end:
        _schedule_full_repaint(view, 0)
        return

      earliest_old_start = min(earliest_old_start, old_start)
      latest_old_end     = max(latest_old_end, old_end)
      previous_old_end   = old_end
      total_delta       += len(change.str) - (old_end - old_start)

    if state.size + total_delta != view.size():
      _schedule_full_repaint(view, 0)
      return

    dirty_segment_index  = 0
    segment_search_count = 0

    while dirty_segment_index + 1 < len(old_segments) and old_segments[dirty_segment_index].end <= earliest_old_start:
      if segment_search_count >= MAX_SEGMENT_SEARCH:
        _schedule_full_repaint(view, 0)
        return

      dirty_segment_index  += 1
      segment_search_count += 1

    current_start             = old_segments[dirty_segment_index].start
    stack                     = old_segments[dirty_segment_index].start_stack
    next_segment_id           = state.next_segment_id
    new_segments              = []
    consecutive_matches       = 0
    convergence_old_index     = None
    incremental_segment_count = 0
    scanned_characters        = 0

    # ->> A single net delta maps checkpoints only after the final changed range.
    first_candidate_index = dirty_segment_index
    segment_search_count  = 0

    while first_candidate_index < len(old_segments) - 1 and old_segments[first_candidate_index].end < latest_old_end:
      if segment_search_count >= MAX_SEGMENT_SEARCH:
        _schedule_full_repaint(view, 0)
        return

      first_candidate_index += 1
      segment_search_count  += 1

    try:
      for candidate_index in range(first_candidate_index, len(old_segments) - 1):
        old_candidate = old_segments[candidate_index]
        candidate_end = old_candidate.end + total_delta

        if candidate_end < current_start or candidate_end > view.size():
          raise ValueError("invalid transformed checkpoint")

        while current_start + CHECKPOINT_INTERVAL < candidate_end:
          if incremental_segment_count >= MAX_INCREMENTAL_SEGMENTS:
            _schedule_full_repaint(view, 0)
            return

          end                 = current_start + CHECKPOINT_INTERVAL
          scanned_characters += end - current_start

          if scanned_characters > MAX_INCREMENTAL_SCAN:
            _schedule_full_repaint(view, 0)
            return

          end_stack, regions_by_depth, _ = _scan_segment(view, current_start, end, stack)
          new_segments.append(Segment(next_segment_id, current_start, end, stack, end_stack, _scope_fingerprint(view, end), regions_by_depth))
          current_start              = end
          incremental_segment_count += 1
          next_segment_id           += 1
          stack                      = end_stack

        if current_start < candidate_end:
          if incremental_segment_count >= MAX_INCREMENTAL_SEGMENTS:
            _schedule_full_repaint(view, 0)
            return

          scanned_characters += candidate_end - current_start
          if scanned_characters > MAX_INCREMENTAL_SCAN:
            _schedule_full_repaint(view, 0)
            return

          end_stack, regions_by_depth, _ = _scan_segment(view, current_start, candidate_end, stack)
          new_segments.append(Segment(next_segment_id, current_start, candidate_end, stack, end_stack, _scope_fingerprint(view, candidate_end), regions_by_depth))
          current_start              = candidate_end
          incremental_segment_count += 1
          next_segment_id           += 1
          stack                      = end_stack

        if stack == old_candidate.end_stack and _scope_fingerprint(view, candidate_end) == old_candidate.end_fingerprint:
          consecutive_matches += 1
        else:
          consecutive_matches = 0

        # ->> Two matching checkpoints greatly reduce accidental convergence after syntax-scope propagation.
        if consecutive_matches >= 2:
          convergence_old_index = candidate_index
          break

      suffix_segments = []

      if convergence_old_index is None:
        while current_start < view.size():
          if incremental_segment_count >= MAX_INCREMENTAL_SEGMENTS:
            _schedule_full_repaint(view, 0)
            return

          end                 = min(view.size(), current_start + CHECKPOINT_INTERVAL)
          scanned_characters += end - current_start

          if scanned_characters > MAX_INCREMENTAL_SCAN:
            _schedule_full_repaint(view, 0)
            return

          end_stack, regions_by_depth, _ = _scan_segment(view, current_start, end, stack)
          new_segments.append(Segment(next_segment_id, current_start, end, stack, end_stack, _scope_fingerprint(view, end), regions_by_depth))
          current_start              = end
          incremental_segment_count += 1
          next_segment_id           += 1
          stack                      = end_stack

        old_segments_to_erase = old_segments[dirty_segment_index:]
      else:
        for old_segment in old_segments[convergence_old_index + 1:]:
          suffix_segments.append(Segment(old_segment.key_id, old_segment.start + total_delta, old_segment.end + total_delta, old_segment.start_stack, old_segment.end_stack, old_segment.end_fingerprint, old_segment.regions_by_depth))

        old_segments_to_erase = old_segments[dirty_segment_index:convergence_old_index + 1]

      # ->> A converged suffix already carries the old EOF sentinel forward.
      if not suffix_segments:
        new_segments.append(Segment(next_segment_id, view.size(), view.size(), stack, stack, _scope_fingerprint(view, view.size()), [[] for _ in COLORS]))
        next_segment_id += 1

      if view.change_count() != current_change_count:
        _schedule_full_repaint(view, 0)
        return

      use_overlay_scopes, wrote_overlay = _ensure_color_scheme_overlay(view)

      for segment in old_segments_to_erase:
        _erase_segment(view, segment)

      for segment in new_segments:
        _paint_segment(view, segment, use_overlay_scopes)

      state.change_count    = current_change_count
      state.next_segment_id = next_segment_id
      state.segments        = old_segments[:dirty_segment_index] + new_segments + suffix_segments
      state.size            = view.size()

      if wrote_overlay:
        sublime.set_timeout_async(lambda: _reapply_existing_segments(view), 120)

    except Exception as error:
      print("Polychrome: Incremental repaint failed - " + str(error))
      _schedule_full_repaint(view, 0)

def _is_eligible_view(view):
  return view is not None and view.is_valid() and not view.is_loading() and not view.settings().get("is_widget", False)

def _paint_segment(view, segment, use_overlay_scopes):
  scopes = POLYCHROME_SCOPES if use_overlay_scopes else REGION_SCOPES

  for depth, offsets in enumerate(segment.regions_by_depth):
    key = _segment_key(segment, depth)

    if not offsets:
      view.erase_regions(key)
      continue

    view.add_regions(key, [sublime.Region(segment.start + offset, segment.start + offset + 1) for offset in offsets], scopes[depth], "", REGION_FLAGS)

def _reapply_existing_segments(view):
  if not _is_eligible_view(view):
    return

  state = _get_state(view)
  if state is None:
    return

  use_overlay_scopes, _ = _ensure_color_scheme_overlay(view)

  with state.lock:
    for segment in state.segments:
      _paint_segment(view, segment, use_overlay_scopes)

def _scan_segment(view, start, end, start_stack, ignored_regions=None, ignored_region_index=0):
  color_count                  = len(COLORS)
  ignored_region_advance_count = 0
  ignored_region_count         = len(ignored_regions) if ignored_regions is not None else 0
  match_selector               = view.match_selector
  regions_by_depth             = [[] for _ in COLORS]
  stack                        = list(start_stack)
  text                         = view.substr(sublime.Region(start, end))

  for match in DELIMITER_PATTERN.finditer(text):
    offset = match.start()
    point  = start + offset

    if ignored_regions is None:
      if match_selector(point, IGNORED_SCOPE_SELECTOR):
        continue
    else:
      while ignored_region_index < ignored_region_count and ignored_regions[ignored_region_index][1] <= point:
        if ignored_region_advance_count >= MAX_IGNORED_REGION_ADVANCES:
          ignored_regions = None
          break

        ignored_region_advance_count += 1
        ignored_region_index         += 1

      if ignored_regions is None:
        if match_selector(point, IGNORED_SCOPE_SELECTOR):
          continue
      elif ignored_region_index < ignored_region_count and ignored_regions[ignored_region_index][0] <= point < ignored_regions[ignored_region_index][1]:
        continue

    delimiter         = match.group(0)
    opening_delimiter = DELIMITER_MAP.get(delimiter)

    if opening_delimiter is None:
      regions_by_depth[len(stack) % color_count].append(offset)
      stack.append(delimiter)
      continue

    if stack and opening_delimiter == stack[-1]:
      regions_by_depth[(len(stack) - 1) % color_count].append(offset)
      stack.pop()

  return "".join(stack), regions_by_depth, ignored_region_index

def _schedule_full_repaint(view, delay=60, commanded=False, repaint_retry_count=0):
  if view is None or not view.is_valid():
    return

  view_id = view.id()

  with STATE_DICTIONARY_LOCK:
    generation                = PENDING_REPAINTS.get(view_id, 0) + 1
    PENDING_REPAINTS[view_id] = generation

  def repaint_if_current():
    with STATE_DICTIONARY_LOCK:
      if PENDING_REPAINTS.get(view_id) != generation:
        return

      PENDING_REPAINTS.pop(view_id, None)

    _full_repaint(view, commanded, repaint_retry_count)

  sublime.set_timeout_async(repaint_if_current, delay)

def _scope_fingerprint(view, point):
  size = view.size()

  if not size:
    return "", "", ""

  return tuple(view.scope_name(scope_point) for scope_point in (
    max(0, min(size - 1, point - 1)),
    max(0, min(size - 1, point)),
    max(0, min(size - 1, point + 1))
  ))

def _segment_key(segment, depth):
  return "polychrome.segment." + str(segment.key_id) + ".depth." + str(depth)

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

  _write_text_if_changed(
    os.path.join(package_directory, "package-metadata.json"),
    json.dumps({
      "description" : "Performant rainbow nesting for Sublime Text 3 and later",
      "name"        : PACKAGE_NAME,
      "platforms"   : ["*"],
      "sublime_text": ">=3148",
      "version"     : PACKAGE_VERSION
    }, indent=2, sort_keys=True) + "\n"
  )

  for window in sublime.windows():
    for view in window.views():
      _schedule_full_repaint(view, 0)


def plugin_unloaded():
  with STATE_DICTIONARY_LOCK:
    states = dict(STATES)

    PENDING_REPAINTS.clear()
    STATES.clear()

  for window in sublime.windows():
    for view in window.views():
      state = states.get(view.id())

      if state is None:
        continue

      for segment in state.segments:
        _erase_segment(view, segment)

# ...
if HAS_TEXT_CHANGE_LISTENER:
  class PolychromeTextChangeListener(sublime_plugin.TextChangeListener):
    @classmethod
    def is_applicable(cls, buffer):
      view = buffer.primary_view()
      return view is not None and not view.settings().get("is_widget", False)

    def on_text_changed(self, changes):
      # ->> `Buffer.views()` and `TextChangeListener` both exist in Sublime Text 4 build 4081+, avoiding the slower all-window/ -view search and `Buffer.id()`, which was not added until build 4083.
      for view in self.buffer.views():
        _incremental_repaint(view, changes)
