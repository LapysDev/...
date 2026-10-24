# %AppData%/Sublime Text/Packages/Polychrome/polychrome.py
import io, json, os, re
import sublime, sublime_plugin

# ...
BUFFER_CHARACTER_LIMIT = 67108864 # --> 64M characters
COLORS                 = ("#FF6060", "#FF9F3F", "#FFCF60", "#60CF60", "#60FFFF", "#6090FF", "#C060FF", "#FF60C0")
DELIMITERS             = (("(", ")"), ("[", "]"), ("{", "}"), ("᚛", "᚜"), ("‹", "›"), ("«", "»"), ("❨", "❩"), ("❪", "❫"), ("❬", "❭"), ("❮", "❯"), ("❰", "❱"), ("❲", "❳"), ("❴", "❵"), ("⟅", "⟆"), ("⟦", "⟧"), ("⟨", "⟩"), ("⟪", "⟫"), ("⟬", "⟭"), ("⟮", "⟯"), ("⦃", "⦄"), ("⦅", "⦆"), ("⦗", "⦘"), ("⧘", "⧙"), ("⧚", "⧛"), ("⸨", "⸩"), ("〈", "〉"), ("《", "》"), ("「", "」"), ("『", "』"), ("【", "】"), ("〔", "〕"), ("〖", "〗"), ("〘", "〙"), ("〚", "〛"), ("﴾", "﴿"), ("﹙", "﹚"), ("﹛", "﹜"), ("﹝", "﹞"), ("（", "）"), ("［", "］"), ("｛", "｝"), ("｟", "｠"), ("｢", "｣"))
DELIMITER_MAP          = dict((closing, opening) for opening, closing in DELIMITERS)
DELIMITER_SET          = frozenset("".join(opening + closing for opening, closing in DELIMITERS))
IGNORED_SCOPE_SELECTOR = "comment, constant"
PACKAGE_NAME           = "Polychrome"
PACKAGE_VERSION        = "3.4.1"
MULTI_LINE_QUOTES      = (("'", "'"), ("\"", "\""), ("“", "”"), ("`", "`"), ("ˋ", "ˊ"), ("⹁", ","), ("❛", "❜"), ("❝", "❞"), ("«", "»"), ("「", "」"), ("『", "』"))
SINGLE_LINE_QUOTES     = (("/", "/"), ("‘", "’"))
SINGLE_LINE_QUOTE_MAP  = dict(SINGLE_LINE_QUOTES)
QUOTE_MAP              = dict(MULTI_LINE_QUOTES + SINGLE_LINE_QUOTES)
QUOTE_CHARACTERS       = frozenset("".join(opening + closing for opening, closing in MULTI_LINE_QUOTES + SINGLE_LINE_QUOTES))
TOKEN_PATTERN          = re.compile("[" + re.escape("".join(opening + closing for opening, closing in DELIMITERS + MULTI_LINE_QUOTES + SINGLE_LINE_QUOTES) + "\\\r\n") + "]")
POLYCHROME_SCOPES      = tuple("polychrome.depth." + str(depth) for depth in range(len(COLORS)))
REGION_FLAGS           = sublime.DRAW_NO_OUTLINE | getattr(sublime, "HIDE_ON_MINIMAP", 0)
REGION_KEYS            = tuple("polychrome.depth." + str(depth) for depth in range(len(COLORS)))
REGION_SCOPES          = ("region.redish", "region.orangish", "region.yellowish", "region.greenish", "region.cyanish", "region.bluish", "region.purplish", "region.pinkish")
REPAINT_DELAY          = 150 # --> Debounce edits before scanning
REPAINT_JOBS           = {}  # --> One current scan per view; no content cache
REPAINT_STEP_DELAY     = 2   # --> Yield the asynchronous callback queue between chunks
SCAN_CHUNK_SIZE        = 16384 # --> 16K characters
STATUS_KEY             = "polychrome.progress"
VIEWPORT_POLL_DELAY    = 250 # --> Scroll events have no dedicated Sublime Text API callback
VIEWPORT_POLLING       = False

# ...
class PolychromeEventListener(sublime_plugin.EventListener):
  def on_close(self, view):
    REPAINT_JOBS.pop(view.id(), None)
    if view.is_valid():
      view.erase_status(STATUS_KEY)

  def on_activated_async(self, view):
    if _is_eligible_view(view):
      _schedule_repaint(view, 0, True)

  def on_clone_async(self, view):
    _schedule_repaint(view, 0, True)

  def on_load_async(self, view):
    _schedule_repaint(view, 0, True)

  def on_modified_async(self, view):
    _schedule_repaint(view)

  def on_new_async(self, view):
    _schedule_repaint(view, 0, True)

  def on_post_save_async(self, view):
    _schedule_repaint(view, 0)

  def on_reload_async(self, view):
    _schedule_repaint(view, 0, True)

  def on_revert_async(self, view):
    _schedule_repaint(view, 0, True)

# ...
def _ensure_color_scheme_overlay(view):
  if int(sublime.version()) < 3150:
    return False, False

  background            = "#000000"
  color_scheme_resource = view.settings().get("color_scheme")

  if not color_scheme_resource:
    return False, False

  filename = os.path.splitext(color_scheme_resource.replace("\\", "/").rsplit("/", 1)[-1])[0] + ".sublime-color-scheme"

  try:
    background = view.style().get("background", background)
  except Exception:
    return False, False

  nearest_background = background

  if not isinstance(background, str):
    nearest_background = "#000001"
  elif len(background) == 7 and background.startswith("#"):
    try:
      color_value = int(background[1:], 16)
    except ValueError:
      pass
    else:
      blue               = (color_value >> 0)  & 0xFF
      green              = (color_value >> 8)  & 0xFF
      red                = (color_value >> 16) & 0xFF
      nearest_background = "#" + hex(red + 1 if red < 255 else red - 1)[2:].upper().zfill(2) + hex(green)[2:].upper().zfill(2) + hex(blue)[2:].upper().zfill(2)

  rules = []

  for depth, foreground in enumerate(COLORS):
    rules.append({"background": nearest_background, "foreground": foreground, "name": "Polychrome depth " + str(depth + 1), "scope": POLYCHROME_SCOPES[depth]})

  return True, _write_text_if_changed(
    os.path.join(sublime.packages_path(), "User", "Color Schemes", PACKAGE_NAME, filename),
    json.dumps({"name": "Polychrome generated overlay", "rules": rules}, indent=2, sort_keys=True) + "\n"
  )

def _erase_regions(view):
  for region_key in REGION_KEYS:
    view.erase_regions(region_key)

def _is_eligible_view(view):
  return view is not None and view.is_valid() and not view.is_loading() and not view.settings().get("is_widget", False)

def _paint(view, change_count, regions_by_depth, use_overlay_scopes):
  completed = 0
  total     = max(1, sum(len(points) for points in regions_by_depth))
  scopes    = POLYCHROME_SCOPES if use_overlay_scopes else REGION_SCOPES

  _set_status(view, "Painting", 81)

  for depth, points in enumerate(regions_by_depth):
    regions = []

    for position in range(0, len(points), SCAN_CHUNK_SIZE):
      if not _is_eligible_view(view) or view.change_count() != change_count:
        return

      regions.extend(sublime.Region(point, point + 1) for point in points[position:position + SCAN_CHUNK_SIZE])
      _set_status(view, "Painting", 81 + 18 * (completed + len(regions)) // total)
      yield REPAINT_STEP_DELAY

    if not _is_eligible_view(view) or view.change_count() != change_count:
      return

    if regions:
      view.add_regions(REGION_KEYS[depth], regions, scopes[depth], "", REGION_FLAGS)
    else:
      view.erase_regions(REGION_KEYS[depth])

    completed += len(points)
    yield REPAINT_STEP_DELAY

def _paint_matched_string_delimiters(regions_by_depth, string_delimiters, paint_begin):
  # ->> Count only completed pairs when calculating rainbow depth. Unmatched opening delimiters must neither be coloured nor affect nested pair colours
  depth = 0

  for point, is_opening, matched in string_delimiters:
    if not matched:
      continue

    if is_opening:
      if point >= paint_begin:
        regions_by_depth[depth % len(COLORS)].append(point)
      depth += 1
    else:
      depth -= 1
      if point >= paint_begin:
        regions_by_depth[depth % len(COLORS)].append(point)

def _has_closing_quote(view, point, quote, scan_limit):
  escaped          = False
  last_token_point = point
  end              = min(scan_limit, view.line(point).end()) if quote in SINGLE_LINE_QUOTE_MAP else scan_limit
  closing          = QUOTE_MAP[quote]

  position = point + 1

  while position < end:
    chunk_end   = min(position + SCAN_CHUNK_SIZE, end)
    scope_index = 0
    scope_end   = position
    ignored     = False
    scopes      = view.extract_tokens_with_scopes(sublime.Region(position, chunk_end))

    for match in TOKEN_PATTERN.finditer(view.substr(sublime.Region(position, chunk_end))):
      token_point = position + match.start()
      token       = match.group(0)

      if token_point >= scope_end:
        while scope_index < len(scopes) and token_point >= scopes[scope_index][0].end():
          scope_index += 1

        if scope_index < len(scopes):
          scope_end = scopes[scope_index][0].end()
          ignored   = sublime.score_selector(scopes[scope_index][1], IGNORED_SCOPE_SELECTOR) > 0
        else:
          scope_end = chunk_end
          ignored   = False

      if token_point != last_token_point + 1:
        escaped = False
      last_token_point = token_point

      if ignored:
        escaped = False
        continue

      if token == "\\":
        escaped = not escaped
        continue

      if token == "`" and not escaped and quote != "`" and token_point + 1 < end and view.substr(token_point + 1) in QUOTE_CHARACTERS:
        escaped = True
        continue

      if escaped:
        escaped = False
        continue

      if token == closing:
        return True

    position = chunk_end
    yield REPAINT_STEP_DELAY

  return False

def _repaint(view, expected_change_count, scan_limit, paint_begin):
  if not _is_eligible_view(view):
    return

  change_count = view.change_count()

  if change_count != expected_change_count:
    return

  if scan_limit > BUFFER_CHARACTER_LIMIT:
    _erase_regions(view)
    return "Skipped (file too large)"

  # ->> Keep the old colours during scanning; replace them only when complete.
  escaped           = False
  last_token_point  = -2
  position          = 0
  quote             = None
  regions_by_depth  = [[] for _ in COLORS]
  size              = scan_limit
  stack             = []
  string_delimiters = []
  string_stack      = []

  _set_status(view, "Scanning", 1)

  while position < size:
    if view.change_count() != change_count:
      return

    end         = min(size, position + SCAN_CHUNK_SIZE)
    text        = view.substr(sublime.Region(position, end))
    scope_index = 0
    scope_end   = position
    ignored     = False
    scopes      = view.extract_tokens_with_scopes(sublime.Region(position, end))

    for match in TOKEN_PATTERN.finditer(text):
      point = position + match.start()
      token = match.group(0)

      if point >= scope_end:
        while scope_index < len(scopes) and point >= scopes[scope_index][0].end():
          scope_index += 1

        if scope_index < len(scopes):
          scope_end = scopes[scope_index][0].end()
          ignored   = sublime.score_selector(scopes[scope_index][1], IGNORED_SCOPE_SELECTOR) > 0
        else:
          scope_end = end
          ignored   = False

      if point != last_token_point + 1:
        escaped = False
      last_token_point = point

      if token == "\n" or token == "\r":
        if quote in SINGLE_LINE_QUOTE_MAP:
          _paint_matched_string_delimiters(regions_by_depth, string_delimiters, paint_begin)
          quote             = None
          string_stack      = []
          string_delimiters = []
        escaped = False
        continue

      if ignored:
        escaped = False
        continue

      if token == "\\":
        escaped = not escaped
        continue

      # ->> Enclosed backticks take precedence over backtick-prefixed quote escapes
      if token == "`" and not escaped and quote != "`" and point + 1 < size and view.substr(point + 1) in QUOTE_CHARACTERS and (quote is not None or not (yield from _has_closing_quote(view, point, token, size))):
        escaped = True
        continue

      if escaped:
        escaped = False
        if quote is not None or token not in QUOTE_MAP or not (yield from _has_closing_quote(view, point, token, size)):
          continue

      if quote is not None:
        if token == QUOTE_MAP[quote]:
          _paint_matched_string_delimiters(regions_by_depth, string_delimiters, paint_begin)
          quote             = None
          string_stack      = []
          string_delimiters = []
          continue

        if token not in DELIMITER_SET:
          continue

        opening_delimiter = DELIMITER_MAP.get(token)

        if opening_delimiter is None:
          string_stack.append((token, len(string_delimiters)))
          string_delimiters.append([point, True, False])
        elif string_stack and opening_delimiter == string_stack[-1][0]:
          _, opening_index = string_stack.pop()
          string_delimiters[opening_index][2] = True
          string_delimiters.append((point, False, True))
        continue

      # ->> An unclosed single-line quote is ordinary text; its inner delimiters still belong to the surrounding nest
      if token in SINGLE_LINE_QUOTE_MAP and not (yield from _has_closing_quote(view, point, token, size)):
        continue

      if token in QUOTE_MAP:
        quote             = token
        string_stack      = []
        string_delimiters = []
        continue

      if token not in DELIMITER_SET:
        continue

      opening_delimiter = DELIMITER_MAP.get(token)

      if opening_delimiter is None:
        if point >= paint_begin:
          regions_by_depth[len(stack) % len(COLORS)].append(point)
        stack.append(token)
      elif stack and opening_delimiter == stack[-1]:
        if point >= paint_begin:
          regions_by_depth[(len(stack) - 1) % len(COLORS)].append(point)
        stack.pop()

    position = end
    _set_status(view, "Scanning", 1 + 79 * position // size)
    yield REPAINT_STEP_DELAY

  if quote is not None:
    _paint_matched_string_delimiters(regions_by_depth, string_delimiters, paint_begin)

  if view.change_count() != change_count:
    return

  _set_status(view, "Preparing colours", 80)
  use_overlay_scopes = int(sublime.version()) >= 3150 and bool(view.settings().get("color_scheme"))
  yield from _paint(view, change_count, regions_by_depth, use_overlay_scopes)

def _poll_viewports():
  if not VIEWPORT_POLLING:
    return

  for window in sublime.windows():
    for group in range(window.num_groups()):
      view = window.active_view_in_group(group)

      if _is_eligible_view(view):
        _schedule_repaint(view, 0)

  sublime.set_timeout_async(_poll_viewports, VIEWPORT_POLL_DELAY)

def _set_status(view, action, progress):
  status = PACKAGE_NAME + ": " + action + " " + str(progress) + "%"

  if view.get_status(STATUS_KEY) != status:
    view.set_status(STATUS_KEY, status)

def _schedule_repaint(view, delay=REPAINT_DELAY, ensure_overlay=False):
  if not _is_eligible_view(view):
    return

  # ->> Check the overlay on lifecycle events, not on every edit or scroll.
  if ensure_overlay:
    _, wrote_overlay = _ensure_color_scheme_overlay(view)
    if wrote_overlay:
      delay = max(delay, 120)

  change_count  = view.change_count()
  view_id       = view.id()
  visible       = view.visible_region()
  visible_begin = max(0, min(view.size(), visible.begin()))
  visible_end   = max(visible_begin, min(view.size(), visible.end()))

  # ->> `visible_region()` excludes the minimap; scan and paint one editor page
  # ->> below its lower edge, accounting for wraps and folded lines.
  viewport_position = view.viewport_position()
  viewport_extent   = view.viewport_extent()
  scan_limit        = max(visible_end, view.layout_to_text((viewport_position[0] + viewport_extent[0], viewport_position[1] + 2 * viewport_extent[1])))

  if visible_end == visible_begin and view.size():
    scan_limit = max(scan_limit, visible_begin + SCAN_CHUNK_SIZE)

  paint_begin = visible_begin // SCAN_CHUNK_SIZE * SCAN_CHUNK_SIZE
  scan_limit  = min(view.size(), scan_limit)
  revision    = (change_count, paint_begin, scan_limit)

  # ->> Ignore duplicate notifications and supersede old revisions/viewport positions.
  if view_id in REPAINT_JOBS and REPAINT_JOBS[view_id][:3] == revision:
    return

  repaint = _repaint(view, change_count, scan_limit, paint_begin)
  REPAINT_JOBS[view_id] = revision + (repaint,)
  _set_status(view, "Queued", 1)

  def step():
    if REPAINT_JOBS.get(view_id) != revision + (repaint,):
      return

    if not _is_eligible_view(view) or view.change_count() != change_count:
      REPAINT_JOBS.pop(view_id, None)
      if view.is_valid():
        view.erase_status(STATUS_KEY)
      return

    try:
      next_delay = next(repaint)
    except StopIteration as completed:
      if view.change_count() != change_count:
        REPAINT_JOBS.pop(view_id, None)
        view.erase_status(STATUS_KEY)
        return

      REPAINT_JOBS[view_id] = revision + (None,)
      _set_status(view, completed.value or "Complete", 100)
      return

    sublime.set_timeout_async(step, next_delay)

  sublime.set_timeout_async(step, delay)

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
  global VIEWPORT_POLLING

  for window in sublime.windows():
    for view in window.views():
      _schedule_repaint(view, 0, True)

  if not VIEWPORT_POLLING:
    VIEWPORT_POLLING = True
    sublime.set_timeout_async(_poll_viewports, VIEWPORT_POLL_DELAY)

def plugin_unloaded():
  global VIEWPORT_POLLING

  VIEWPORT_POLLING = False
  REPAINT_JOBS.clear()

  for window in sublime.windows():
    for view in window.views():
      if view.is_valid():
        view.erase_status(STATUS_KEY)
        if _is_eligible_view(view):
          _erase_regions(view)
