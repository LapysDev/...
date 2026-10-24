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
REPAINT_DELAY          = 60
SCAN_CHUNK_SIZE        = 16384 # --> 16K characters

# ...
class PolychromeEventListener(sublime_plugin.EventListener):
  def on_activated_async(self, view):
    if _is_eligible_view(view):
      _ensure_color_scheme_overlay(view)

  def on_clone_async(self, view):
    _schedule_repaint(view, 0)

  def on_load_async(self, view):
    _schedule_repaint(view, 0)

  def on_modified_async(self, view):
    _schedule_repaint(view)

  def on_new_async(self, view):
    _schedule_repaint(view, 0)

  def on_post_save_async(self, view):
    _schedule_repaint(view, 0)

  def on_reload_async(self, view):
    _schedule_repaint(view, 0)

  def on_revert_async(self, view):
    _schedule_repaint(view, 0)

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
  if not _is_eligible_view(view) or view.change_count() != change_count:
    return

  scopes = POLYCHROME_SCOPES if use_overlay_scopes else REGION_SCOPES

  for depth, points in enumerate(regions_by_depth):
    if view.change_count() != change_count:
      return

    if points:
      view.add_regions(REGION_KEYS[depth], [sublime.Region(point, point + 1) for point in points], scopes[depth], "", REGION_FLAGS)

def _paint_matched_string_delimiters(regions_by_depth, string_delimiters):
  # ->> Count only completed pairs when calculating rainbow depth. Unmatched opening delimiters must neither be coloured nor affect nested pair colours
  depth = 0

  for point, is_opening, matched in string_delimiters:
    if not matched:
      continue

    if is_opening:
      regions_by_depth[depth % len(COLORS)].append(point)
      depth += 1
    else:
      depth -= 1
      regions_by_depth[depth % len(COLORS)].append(point)

def _has_closing_quote(view, point, quote, ignored_regions, ignored_region_index):
  escaped          = False
  last_token_point = point
  end              = view.line(point).end() if quote in SINGLE_LINE_QUOTE_MAP else view.size()
  closing          = QUOTE_MAP[quote]

  position = point + 1

  while position < end:
    chunk_end = min(position + SCAN_CHUNK_SIZE, end)

    for match in TOKEN_PATTERN.finditer(view.substr(sublime.Region(position, chunk_end))):
      token_point = position + match.start()
      token       = match.group(0)

      if token_point != last_token_point + 1:
        escaped = False
      last_token_point = token_point

      while ignored_region_index < len(ignored_regions) and ignored_regions[ignored_region_index].end() <= token_point:
        ignored_region_index += 1

      if ignored_region_index < len(ignored_regions) and ignored_regions[ignored_region_index].begin() <= token_point < ignored_regions[ignored_region_index].end():
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

  return False

def _repaint(view, expected_change_count):
  if not _is_eligible_view(view):
    return

  change_count = view.change_count()

  if change_count != expected_change_count:
    return

  _erase_regions(view)

  if view.size() > BUFFER_CHARACTER_LIMIT:
    return

  escaped              = False
  ignored_region_index = 0
  ignored_regions      = view.find_by_selector(IGNORED_SCOPE_SELECTOR)
  last_token_point     = -2
  position             = 0
  quote                = None
  regions_by_depth     = [[] for _ in COLORS]
  size                 = view.size()
  stack                = []
  string_stack         = []
  string_delimiters    = []

  ignored_regions.sort(key=lambda region: region.begin())

  while position < size:
    if view.change_count() != change_count:
      return

    end  = min(size, position + SCAN_CHUNK_SIZE)
    text = view.substr(sublime.Region(position, end))

    for match in TOKEN_PATTERN.finditer(text):
      point = position + match.start()
      token = match.group(0)

      if point != last_token_point + 1:
        escaped = False
      last_token_point = point

      if token == "\n" or token == "\r":
        if quote in SINGLE_LINE_QUOTE_MAP:
          _paint_matched_string_delimiters(regions_by_depth, string_delimiters)
          quote             = None
          string_stack      = []
          string_delimiters = []
        escaped = False
        continue

      while ignored_region_index < len(ignored_regions) and ignored_regions[ignored_region_index].end() <= point:
        ignored_region_index += 1

      if ignored_region_index < len(ignored_regions) and ignored_regions[ignored_region_index].begin() <= point < ignored_regions[ignored_region_index].end():
        escaped = False
        continue

      if token == "\\":
        escaped = not escaped
        continue

      # ->> Enclosed backticks take precedence over backtick-prefixed quote escapes
      if token == "`" and not escaped and quote != "`" and point + 1 < size and view.substr(point + 1) in QUOTE_CHARACTERS and (quote is not None or not _has_closing_quote(view, point, token, ignored_regions, ignored_region_index)):
        escaped = True
        continue

      if escaped:
        escaped = False
        if quote is not None or token not in QUOTE_MAP or not _has_closing_quote(view, point, token, ignored_regions, ignored_region_index):
          continue

      if quote is not None:
        if token == QUOTE_MAP[quote]:
          _paint_matched_string_delimiters(regions_by_depth, string_delimiters)
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
      if token in SINGLE_LINE_QUOTE_MAP and not _has_closing_quote(view, point, token, ignored_regions, ignored_region_index):
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
        regions_by_depth[len(stack) % len(COLORS)].append(point)
        stack.append(token)
      elif stack and opening_delimiter == stack[-1]:
        regions_by_depth[(len(stack) - 1) % len(COLORS)].append(point)
        stack.pop()

    position = end

  if quote is not None:
    _paint_matched_string_delimiters(regions_by_depth, string_delimiters)

  if view.change_count() != change_count:
    return

  use_overlay_scopes, wrote_overlay = _ensure_color_scheme_overlay(view)

  if wrote_overlay:
    sublime.set_timeout_async(lambda: _paint(view, change_count, regions_by_depth, use_overlay_scopes), 120)
  else:
    _paint(view, change_count, regions_by_depth, use_overlay_scopes)

def _schedule_repaint(view, delay=REPAINT_DELAY):
  if not _is_eligible_view(view):
    return

  change_count = view.change_count()
  sublime.set_timeout_async(lambda: _repaint(view, change_count), delay)

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
  for window in sublime.windows():
    for view in window.views():
      _schedule_repaint(view, 0)

def plugin_unloaded():
  for window in sublime.windows():
    for view in window.views():
      if _is_eligible_view(view):
        _erase_regions(view)
