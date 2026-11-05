# %AppData%/Sublime Text/Packages/Polychrome/polychrome.py
# Queued → Encoding (Cache) → Parsing → Painting → Ready
from __future__ import annotations
from array      import array
from typing     import Any, Dict, Generator, List, Match, Optional, Set, Tuple

import bisect, io, json, os, re
import sublime, sublime_plugin

# ...
BUFFER_CHARACTER_LIMIT : int                                               = 67108864 # --> 64M characters
CHANGE_LISTENERS       : Dict[int, Any]                                    = {}
CHUNK_SIZE             : int                                               = 16384 # --> Shared encoding and painting chunk size
COLORS                 : Tuple[str, ...]                                   = ("#FF6060", "#FF9F3F", "#FFCF60", "#60CF60", "#60FFFF", "#6090FF", "#C060FF", "#FF60C0")
DELIMITERS             : Tuple[Tuple[str, str], ...]                       = (("(", ")"), ("[", "]"), ("{", "}"), ("᚛", "᚜"), ("‹", "›"), ("«", "»"), ("❨", "❩"), ("❪", "❫"), ("❬", "❭"), ("❮", "❯"), ("❰", "❱"), ("❲", "❳"), ("❴", "❵"), ("⟅", "⟆"), ("⟦", "⟧"), ("⟨", "⟩"), ("⟪", "⟫"), ("⟬", "⟭"), ("⟮", "⟯"), ("⦃", "⦄"), ("⦅", "⦆"), ("⦗", "⦘"), ("⧘", "⧙"), ("⧚", "⧛"), ("⸨", "⸩"), ("〈", "〉"), ("《", "》"), ("「", "」"), ("『", "』"), ("【", "】"), ("〔", "〕"), ("〖", "〗"), ("〘", "〙"), ("〚", "〛"), ("﴾", "﴿"), ("﹙", "﹚"), ("﹛", "﹜"), ("﹝", "﹞"), ("（", "）"), ("［", "］"), ("｛", "｝"), ("｟", "｠"), ("｢", "｣"))
DELIMITER_MAP          : Dict[str, str]                                    = dict((closing, opening) for opening, closing in DELIMITERS)
DELIMITER_SET          : frozenset[str]                                    = frozenset("".join(opening + closing for opening, closing in DELIMITERS))
DOCUMENT_CACHE         : Dict[int, "DocumentCache"]                        = {}
DOCUMENT_VIEWPORT_CACHE: Dict[int, "ViewportCache"]                        = {}
IGNORED_SCOPE_SELECTOR : str                                               = "comment, constant"
MULTI_LINE_QUOTES      : Tuple[Tuple[str, str], ...]                       = (("'", "'"), ("\"", "\""), ("“", "”"), ("`", "`"), ("ˋ", "ˊ"), ("⹁", ","), ("❛", "❜"), ("❝", "❞"), ("«", "»"), ("「", "」"), ("『", "』"))
PACKAGE_NAME           : str                                               = "Polychrome"
PACKAGE_VERSION        : str                                               = "3.5.2"
PENDING_CHANGES        : Dict[int, Tuple[int, List[Tuple[int, int, str]]]] = {}
POSITION_LIMIT         : int                                               = (1 << 32) - 1 # --> Unsigned 32-bit character positions
POLYCHROME_SCOPES      : Tuple[str, ...]                                   = tuple("polychrome.depth." + str(depth) for depth in range(len(COLORS)))
QUOTE_MAP              : Dict[str, str]                                    = dict(MULTI_LINE_QUOTES + (("/", "/"), ("‘", "’")))
REGION_FLAGS           : int                                               = sublime.DRAW_NO_OUTLINE | getattr(sublime, "HIDE_ON_MINIMAP", 0)
REGION_KEYS            : Tuple[str, ...]                                   = POLYCHROME_SCOPES
REGION_SCOPES          : Tuple[str, ...]                                   = ("region.redish", "region.orangish", "region.yellowish", "region.greenish", "region.cyanish", "region.bluish", "region.purplish", "region.pinkish")
REPAINT_DELAY          : int                                               = 150
REPAINT_JOBS           : Dict[int, Tuple[Any, ...]]                        = {}
REPAINT_STEP_DELAY     : int                                               = 2
SINGLE_LINE_QUOTES     : Tuple[Tuple[str, str], ...]                       = (("/", "/"), ("‘", "’"))
SINGLE_LINE_QUOTE_MAP  : Dict[str, str]                                    = dict(SINGLE_LINE_QUOTES)
STATUS_KEY             : str                                               = "polychrome.progress"
SYNTAX_HIGHLIGHT_LIMIT : int                                               = 16384
TOKEN_CHARACTERS       : Tuple[str, ...]                                   = tuple(sorted(DELIMITER_SET | frozenset("".join(opening + closing for opening, closing in MULTI_LINE_QUOTES + SINGLE_LINE_QUOTES)) | frozenset("\\\r\n")))
TOKEN_INDEX            : Dict[str, int]                                    = dict((token, index) for index, token in enumerate(TOKEN_CHARACTERS))
TOKEN_PATTERN          : re.Pattern[str]                                   = re.compile("[" + re.escape("".join(TOKEN_CHARACTERS)) + "]")
VIEWPORT_POLL_DELAY    : int                                               = 250
VIEWPORT_POLLING       : bool                                              = False

# ...
class DocumentCache:
  def __init__(self, revision: int, syntax: Any) -> None:
    self.after        : Tuple[int, int]         = (0, 0)
    self.before       : Tuple[int, int]         = (0, 0)
    self.codes        : array                   = array("I")
    self.covered      : int                     = 0
    self.paint_window : Tuple[int, int]         = (-1, -1)
    self.points       : array                   = array("I")
    self.ready        : bool                    = False
    self.revision     : int                     = revision
    self.scope_ids    : Dict[str, int]          = {}
    self.scope_ignored: Dict[str, bool]         = {}
    self.status_bytes : int                     = 0
    self.syntax       : Any                     = syntax
    self.viewport     : Optional[ViewportCache] = None

  def ensure_position_capacity(self, size: int) -> None:
    # ->> Upgrade in place when the full active document exceeds uint32.
    # ->> Never downgrade before this document cache is discarded.
    if size <= POSITION_LIMIT or self.points.typecode == "Q":
      return

    points         : array           = array("Q", self.points)
    viewport_points: Optional[array] = array("Q", self.viewport.points) if self.viewport is not None else None

    self.points = points
    if self.viewport is not None and viewport_points is not None:
      self.viewport.points = viewport_points


# ...
class PolychromeEventListener(sublime_plugin.EventListener):
  def on_activated_async(self, view: sublime.View) -> None:
    _schedule_repaint(view, 0, True)

  def on_clone_async(self, view: sublime.View) -> None:
    _schedule_repaint(view, 0, True)

  def on_close(self, view: sublime.View) -> None:
    DOCUMENT_CACHE.pop(view.id(), None)
    DOCUMENT_VIEWPORT_CACHE.pop(view.id(), None)
    PENDING_CHANGES.pop(view.id(), None)
    REPAINT_JOBS.pop(view.id(), None)
    _prune_inactive()

  def on_deactivated_async(self, view: sublime.View) -> None:
    _prune_inactive()

  def on_load_async(self, view: sublime.View) -> None:
    DOCUMENT_CACHE.pop(view.id(), None)
    DOCUMENT_VIEWPORT_CACHE.pop(view.id(), None)
    PENDING_CHANGES.pop(view.id(), None)
    REPAINT_JOBS.pop(view.id(), None)
    _schedule_repaint(view, 0, True)

  def on_modified_async(self, view: sublime.View) -> None:
    _schedule_repaint(view)

  def on_new_async(self, view: sublime.View) -> None:
    _schedule_repaint(view, 0, True)

  def on_post_save_async(self, view: sublime.View) -> None:
    _schedule_repaint(view, 0)

  def on_reload_async(self, view: sublime.View) -> None:
    DOCUMENT_CACHE.pop(view.id(), None)
    DOCUMENT_VIEWPORT_CACHE.pop(view.id(), None)
    PENDING_CHANGES.pop(view.id(), None)
    REPAINT_JOBS.pop(view.id(), None)
    _schedule_repaint(view, 0, True)

  def on_revert_async(self, view: sublime.View) -> None:
    DOCUMENT_CACHE.pop(view.id(), None)
    DOCUMENT_VIEWPORT_CACHE.pop(view.id(), None)
    PENDING_CHANGES.pop(view.id(), None)
    REPAINT_JOBS.pop(view.id(), None)
    _schedule_repaint(view, 0, True)

  def on_syntax_changed_async(self, view: sublime.View) -> None:
    DOCUMENT_CACHE.pop(view.id(), None)
    _schedule_repaint(view, 0)

class PolychromeRepaintCommand(sublime_plugin.ApplicationCommand):
  def run(self) -> None:
    _reset_all()
    for window in sublime.windows():
      window: sublime.Window
      for group in range(window.num_groups()):
        group: int
        view : Optional[sublime.View] = window.active_view_in_group(group)
        _schedule_repaint(view, 0, True)

class PolychromeTextChangeListener(getattr(sublime_plugin, "TextChangeListener", object)):
  def on_text_changed_async(self, changes: List[Any]) -> None:
    updates: List[Tuple[int, int, str]] = [(change.a.pt, change.b.pt, change.str) for change in changes]

    for view in self.buffer.views():
      view: sublime.View
      if _is_active_view(view):
        PENDING_CHANGES[view.id()] = (view.change_count(), updates)
        _schedule_repaint(view)

class ViewportCache:
  def __init__(self, begin: int, end: int, revision: int, position_typecode: str = "I") -> None:
    self.begin   : int                                             = begin
    self.codes   : array                                           = array("I")
    self.end     : int                                             = end
    self.layout  : Tuple[Tuple[float, float], Tuple[float, float]] = ((0.0, 0.0), (0.0, 0.0))
    self.points  : array                                           = array(position_typecode)
    self.revision: int                                             = revision


# ...
def _encode(view: sublime.View, cache: DocumentCache, begin: int, end: int) -> Generator[int, None, ViewportCache]:
  # ->> Positions follow the document width; token/scope codes remain uint32.
  segment: ViewportCache = ViewportCache(begin, end, view.change_count(), cache.points.typecode)

  for position in range(begin, end, CHUNK_SIZE):
    position   : int
    chunk_end  : int                                        = min(position + CHUNK_SIZE, end)
    scope_end  : int                                        = position
    scope_index: int                                        = 0
    scope_name : str                                        = ""
    scopes     : Optional[List[Tuple[sublime.Region, str]]] = None
    text       : str                                        = view.substr(sublime.Region(position, chunk_end))

    for match in TOKEN_PATTERN.finditer(text):
      match: Match[str]
      point: int = position + match.start()
      token: str = match.group(0)

      if scopes is None:
        scopes = view.extract_tokens_with_scopes(sublime.Region(position, chunk_end))

      if point >= scope_end:
        while scope_index < len(scopes) and point >= scopes[scope_index][0].end():
          scope_index += 1
        if scope_index < len(scopes):
          scope_end  = scopes[scope_index][0].end()
          scope_name = scopes[scope_index][1]
        else:
          scope_end  = chunk_end
          scope_name = ""

      if scope_name not in cache.scope_ignored:
        cache.scope_ignored[scope_name] = sublime.score_selector(scope_name, IGNORED_SCOPE_SELECTOR) > 0
      if token not in ("\r", "\n") and cache.scope_ignored[scope_name]:
        continue

      if scope_name not in cache.scope_ids:
        cache.scope_ids[scope_name] = len(cache.scope_ids)

      # ->> Eight low bits encode the character; upper bits identify its full syntax scope.
      segment.codes.append((cache.scope_ids[scope_name] << 8) | TOKEN_INDEX[token])
      segment.points.append(point)

    _set_status(view, "Encoding", 1 + 8 * (chunk_end - begin) // max(1, end - begin))
    yield REPAINT_STEP_DELAY

  return segment

def _is_active_view(view: Optional[sublime.View]) -> bool:
  if view is None or not view.is_valid() or view.is_loading() or view.settings().get("is_widget", False):
    return False

  window: Optional[sublime.Window] = view.window()
  if window is None:
    return False

  for group in range(window.num_groups()):
    group      : int
    active_view: Optional[sublime.View] = window.active_view_in_group(group)
    if active_view is not None and active_view.id() == view.id():
      return True

  return False

def _paint(view: sublime.View, revision: int, points_by_depth: List[List[int]]) -> Generator[int, None, None]:
  scopes: Tuple[str, ...] = POLYCHROME_SCOPES if int(sublime.version()) >= 3150 and bool(view.settings().get("color_scheme")) else REGION_SCOPES

  for depth, points in enumerate(points_by_depth):
    depth  : int
    points : List[int]
    regions: List[sublime.Region] = [sublime.Region(point, point + 1) for point in points]

    if not _is_active_view(view) or view.change_count() != revision:
      return

    if view.get_regions(REGION_KEYS[depth]) != regions:
      if regions:
        view.add_regions(REGION_KEYS[depth], regions, scopes[depth], "", REGION_FLAGS)
      else:
        view.erase_regions(REGION_KEYS[depth])

    _set_status(view, "Painting", 81 + 18 * (depth + 1) // len(COLORS))
    yield REPAINT_STEP_DELAY

def _paint_string_pairs(view: sublime.View, entries: List[List[Any]], points_by_depth: List[List[int]], begin: int, end: int) -> None:
  # ->> Matched strings alone contribute nesting depth; unmatched openings do not.
  depth: int = 0

  for point, opening, matched, paintable in entries:
    point    : int
    opening  : bool
    matched  : bool
    paintable: bool
    if not matched:
      continue
    if opening:
      if begin <= point < end and paintable:
        points_by_depth[depth % len(COLORS)].append(point)
      depth += 1
    else:
      depth -= 1
      if begin <= point < end and paintable:
        points_by_depth[depth % len(COLORS)].append(point)

def _parse(view: sublime.View, cache: DocumentCache, revision: int, begin: int, end: int) -> Generator[int, None, None]:
  # ->> Parse only the validated prefix; produce regions only in the bounded paint window.
  closing_positions: Dict[str, array]      = dict((quote, array(cache.points.typecode)) for quote in set(QUOTE_MAP.values()))
  escaped          : bool                  = False
  filename         : str                   = (view.file_name() or "").lower() if hasattr(view, "file_name") else ""
  last_point       : int                   = -2
  line_ends        : List[int]             = []
  line_index       : int                   = 0
  line_starts      : List[int]             = [begin]
  newlines         : array                 = array(cache.points.typecode)
  paint_text       : str                   = view.substr(sublime.Region(begin, end))
  paintable        : bool                  = False
  points_by_depth  : List[List[int]]       = [[] for color in COLORS]
  powershell       : bool                  = filename.endswith((".ps1", ".ps1xml", ".psd1", ".psm1", ".psrc", ".pssc")) or "powershell" in str(cache.syntax).lower()
  quote            : Optional[str]         = None
  stack            : List[str]             = []
  string_delimiters: List[List[Any]]       = []
  string_stack     : List[Tuple[str, int]] = []

  for match in re.finditer("[\r\n]", paint_text):
    match: Match[str]
    line_ends.append(begin + match.start())
    line_starts.append(begin + match.end())
  line_ends.append(end)

  # ->> Syntax modes and embedded PowerShell are recognised independently of extensions.
  if not powershell and hasattr(view, "match_selector"):
    try:
      powershell = bool(view.match_selector(min(begin, max(0, view.size() - 1)), "source.powershell"))
    except Exception:
      pass

  for index, point in enumerate(cache.points):
    index: int
    point: int
    if point >= end:
      break
    if index and (index & (CHUNK_SIZE - 1)) == 0:
      yield REPAINT_STEP_DELAY
    token: str = TOKEN_CHARACTERS[cache.codes[index] & 0xFF]
    if point != last_point + 1:
      escaped = False
    last_point = point
    if token == "\\":
      escaped = not escaped
      continue
    if token == "\n" or token == "\r":
      newlines.append(point)
    if not escaped and token in closing_positions:
      closing_positions[token].append(point)
    escaped = False

  escaped    = False
  last_point = -2
  _set_status(view, "Parsing", 10)

  for index, point in enumerate(cache.points):
    index: int
    point: int
    if point >= end:
      break
    if index and (index & (CHUNK_SIZE - 1)) == 0:
      _set_status(view, "Parsing", 10 + 70 * point // max(1, end))
      yield REPAINT_STEP_DELAY

    token: str = TOKEN_CHARACTERS[cache.codes[index] & 0xFF]
    if point != last_point + 1:
      escaped = False
    last_point = point

    paintable = False
    if begin <= point < end:
      while line_index + 1 < len(line_starts) and line_starts[line_index + 1] <= point:
        line_index += 1
      paintable = line_ends[line_index] - line_starts[line_index] < SYNTAX_HIGHLIGHT_LIMIT

    if token == "\n" or token == "\r":
      if quote in SINGLE_LINE_QUOTE_MAP:
        _paint_string_pairs(view, string_delimiters, points_by_depth, begin, end)
        quote             = None
        string_delimiters = []
        string_stack      = []
      escaped = False
      continue

    if token == "\\":
      escaped = not escaped
      continue

    if powershell and token == "`" and not escaped and quote != "`" and point + 1 < end and view.substr(point + 1) in QUOTE_MAP and (quote is not None or not closing_positions.get("`") or bisect.bisect_right(closing_positions["`"], point) >= len(closing_positions["`"])):
      escaped = True
      continue

    if escaped:
      escaped = False
      continue

    if quote is not None:
      if token == QUOTE_MAP[quote]:
        _paint_string_pairs(view, string_delimiters, points_by_depth, begin, end)
        quote             = None
        string_delimiters = []
        string_stack      = []
        continue

      if token in DELIMITER_SET:
        opening: Optional[str] = DELIMITER_MAP.get(token)
        if opening is None:
          string_stack.append((token, len(string_delimiters)))
          string_delimiters.append([point, True, False, paintable])
        elif string_stack and string_stack[-1][0] == opening:
          opening_index: int = string_stack.pop()[1]
          string_delimiters[opening_index][2] = True
          string_delimiters.append([point, False, True, paintable])
      continue

    if token in QUOTE_MAP:
      if token in SINGLE_LINE_QUOTE_MAP:
        closers      : array = closing_positions[QUOTE_MAP[token]]
        next_pos     : int   = bisect.bisect_right(closers, point)
        line_position: int   = bisect.bisect_left(newlines, point)
        line_limit   : int   = newlines[line_position] if line_position < len(newlines) else end
        if next_pos == len(closers) or closers[next_pos] >= line_limit:
          continue
      quote             = token
      string_delimiters = []
      string_stack      = []
      continue

    if token not in DELIMITER_SET:
      continue

    opening: Optional[str] = DELIMITER_MAP.get(token)
    if opening is None:
      if paintable:
        points_by_depth[len(stack) % len(COLORS)].append(point)
      stack.append(token)
    elif stack and stack[-1] == opening:
      if paintable:
        points_by_depth[(len(stack) - 1) % len(COLORS)].append(point)
      stack.pop()


  if quote is not None:
    _paint_string_pairs(view, string_delimiters, points_by_depth, begin, end)

  if _is_active_view(view) and view.change_count() == revision:
    yield from _paint(view, revision, points_by_depth)

def _poll_viewports() -> None:
  if not VIEWPORT_POLLING:
    return

  _prune_inactive()
  for window in sublime.windows():
    window: sublime.Window
    for group in range(window.num_groups()):
      group: int
      view : Optional[sublime.View] = window.active_view_in_group(group)
      _schedule_repaint(view, 0)

  sublime.set_timeout_async(_poll_viewports, VIEWPORT_POLL_DELAY)

def _prune_inactive() -> None:
  # ->> Inactive views are always forgotten, including those never reaching Ready.
  active : Set[int] = set()
  buffers: Set[int] = set()

  for window in sublime.windows():
    window: sublime.Window
    for group in range(window.num_groups()):
      group: int
      view : Optional[sublime.View] = window.active_view_in_group(group)
      if _is_active_view(view):
        active.add(view.id())
        buffers.add(view.buffer_id())

  for view_id in set(DOCUMENT_CACHE) | set(DOCUMENT_VIEWPORT_CACHE) | set(PENDING_CHANGES) | set(REPAINT_JOBS):
    view_id: int
    if view_id in active:
      continue

    DOCUMENT_CACHE.pop(view_id, None)
    DOCUMENT_VIEWPORT_CACHE.pop(view_id, None)
    PENDING_CHANGES.pop(view_id, None)
    REPAINT_JOBS.pop(view_id, None)

    for window in sublime.windows():
      window: sublime.Window
      for view in window.views():
        view: sublime.View
        if view.is_valid() and view.id() == view_id:
          view.erase_status(STATUS_KEY)
          for region_key in REGION_KEYS:
            region_key: str
            view.erase_regions(region_key)

  for buffer_id in list(CHANGE_LISTENERS):
    buffer_id: int
    if buffer_id not in buffers:
      try:
        CHANGE_LISTENERS.pop(buffer_id).detach()
      except Exception:
        pass

def _repaint(view: sublime.View, revision: int, visible_begin: int, visible_end: int, paint_begin: int, paint_end: int, layout: Tuple[Tuple[float, float], Tuple[float, float]]) -> Generator[int, None, None]:
  cache         : Optional[DocumentCache]                          = DOCUMENT_CACHE.get(view.id())
  changes       : Optional[Tuple[int, List[Tuple[int, int, str]]]] = PENDING_CHANGES.get(view.id())
  key_changed   : bool                                             = False
  local         : bool                                             = False
  location_known: bool
  modified      : bool
  old_paint     : Tuple[int, int]
  old_view      : Optional[ViewportCache]
  syntax        : Any                                              = view.settings().get("syntax")

  if cache is None or cache.syntax != syntax:
    cache = DocumentCache(revision, syntax)
    DOCUMENT_CACHE[view.id()] = cache

  # ->> Size-based pessimistic promotion persists even if the document shrinks.
  cache.ensure_position_capacity(view.size())

  # ->> Read only the three-page window to measure its UTF-8 length accurately.
  if cache.paint_window != (paint_begin, paint_end) or cache.revision != revision:
    cache.status_bytes = 0
    for position in range(paint_begin, paint_end, CHUNK_SIZE):
      position: int
      cache.status_bytes += len(view.substr(sublime.Region(position, min(position + CHUNK_SIZE, paint_end))).encode("utf-8"))

  location_known = bool(changes and changes[0] == revision and cache.revision + 1 == revision and changes[1])
  modified       = cache.revision != revision
  old_paint      = cache.paint_window
  old_view       = cache.viewport

  if modified:
    if location_known and old_view is not None:
      local = all(old_view.begin <= begin <= finish <= old_view.end for begin, finish, text in changes[1])

    if local and changes is not None and old_view is not None:
      # ->> The three-part chain leaves the prefix untouched, rebuilds the
      # ->> viewport child, and only shifts suffix positions when length changes.
      lower        : int   = bisect.bisect_left(cache.points, old_view.begin)
      upper        : int   = bisect.bisect_left(cache.points, old_view.end)
      mapped_codes : array = array("I")
      mapped_points: array = array(cache.points.typecode)
      delta        : int   = sum(len(replacement) - (finish - begin) for begin, finish, replacement in changes[1])

      for old_point, code in zip(cache.points[lower:upper], cache.codes[lower:upper]):
        old_point: int
        code     : int
        point    : int  = old_point
        removed  : bool = False
        for begin, finish, replacement in changes[1]:
          begin      : int
          finish     : int
          replacement: str
          if point >= finish:
            point += len(replacement) - (finish - begin)
          elif point >= begin:
            removed = True
            break
        if not removed:
          mapped_points.append(point)
          mapped_codes.append(code)

      if delta:
        cache.points[upper:] = array(cache.points.typecode, (point + delta for point in cache.points[upper:]))
      cache.points[lower:upper] = mapped_points
      cache.codes[lower:upper]  = mapped_codes
      cache.covered += delta
    else:
      # ->> Outside or ambiguous edits require rebuilding from the first edited point.
      start         : int   = min(begin for begin, finish, replacement in changes[1]) if location_known else 0
      previous_start: int   = bisect.bisect_left(cache.points, start)
      old_points    : array = cache.points[previous_start:]
      old_codes     : array = cache.codes[previous_start:]
      del cache.points[previous_start:]
      del cache.codes[previous_start:]
      cache.covered = min(start, cache.covered)

      # ->> Compare the rebuilt suffix with the previous revision; retain the prefix.
      # ->> A previously explored distant page must never expand the current scan.
      target_end: int = paint_end
      updated   : ViewportCache
      if cache.covered < target_end:
        updated = yield from _encode(view, cache, cache.covered, target_end)
      else:
        updated = ViewportCache(cache.covered, target_end, revision, cache.points.typecode)
      if not _is_active_view(view) or view.change_count() != revision:
        return
      cache.points.extend(updated.points)
      cache.codes.extend(updated.codes)
      cache.covered = max(cache.covered, target_end)

      if location_known and changes is not None:
        mapped: List[Tuple[int, int]] = []
        for old_point, code in zip(old_points, old_codes):
          old_point: int
          code     : int
          point    : int  = old_point
          removed  : bool = False
          for begin, finish, replacement in changes[1]:
            begin      : int
            finish     : int
            replacement: str
            if point >= finish:
              point += len(replacement) - (finish - begin)
            elif point >= begin:
              removed = True
              break
          if not removed and point < target_end:
            mapped.append((point, code))
        key_changed = start < paint_end and mapped != list(zip(updated.points, updated.codes))
      else:
        key_changed = True

  # ->> Scrolling forward extends only to the current page-after boundary.
  if cache.covered < paint_end:
    extension: ViewportCache = yield from _encode(view, cache, cache.covered, paint_end)
    if not _is_active_view(view) or view.change_count() != revision:
      return
    cache.points.extend(extension.points)
    cache.codes.extend(extension.codes)
    cache.covered = paint_end

  if cache.viewport is None or modified or cache.viewport.begin != visible_begin or cache.viewport.end != visible_end or cache.viewport.layout != layout:
    # ->> A viewport move or edit recreates the entire small child cache.
    child: ViewportCache = yield from _encode(view, cache, visible_begin, visible_end)
    if not _is_active_view(view) or view.change_count() != revision:
      return

    lower: int = bisect.bisect_left(cache.points, visible_begin)
    upper: int = bisect.bisect_left(cache.points, visible_end)
    if cache.points[lower:upper] != child.points or cache.codes[lower:upper] != child.codes:
      key_changed = True

    # ->> The main chain is [before range, child, after range]. The backing arrays
    # ->> are shared; replacing the child never re-encodes preceding document text.
    cache.points[lower:upper] = child.points
    cache.codes[lower:upper]  = child.codes
    child.layout              = layout
    cache.viewport           = child
    DOCUMENT_VIEWPORT_CACHE[view.id()] = child

    if local and key_changed:
      # ->> A changed quotation/comment boundary can alter scopes after the viewport.
      # ->> Re-validate the adjacent page, but do not reconstruct the entire document.
      lower: int = bisect.bisect_left(cache.points, visible_end)
      del cache.points[lower:]
      del cache.codes[lower:]
      cache.covered = visible_end
      if paint_end > visible_end:
        extension: ViewportCache = yield from _encode(view, cache, visible_end, paint_end)
        if not _is_active_view(view) or view.change_count() != revision:
          return
        cache.points.extend(extension.points)
        cache.codes.extend(extension.codes)
        cache.covered = paint_end

  if cache.viewport is not None:
    lower: int = bisect.bisect_left(cache.points, cache.viewport.begin)
    upper: int = bisect.bisect_left(cache.points, cache.viewport.end)
    cache.before = (0, lower)
    cache.after  = (upper, len(cache.points))

  cache.revision = revision
  PENDING_CHANGES.pop(view.id(), None)

  # ->> Unchanged significant tokens and an unchanged three-page paint window
  # ->> need neither the rainbow parser nor Sublime region updates.
  if not key_changed and cache.ready and old_paint == (paint_begin, paint_end) and old_view is not None:
    cache.ready = True
    _set_status(view, "Ready", 100)
    return

  cache.ready = False
  yield from _parse(view, cache, revision, paint_begin, paint_end)
  if _is_active_view(view) and view.change_count() == revision:
    cache.paint_window = (paint_begin, paint_end)
    cache.ready        = True
    _set_status(view, "Ready", 100)

def _reset_all() -> None:
  DOCUMENT_CACHE.clear()
  DOCUMENT_VIEWPORT_CACHE.clear()
  PENDING_CHANGES.clear()
  REPAINT_JOBS.clear()

  for listener in CHANGE_LISTENERS.values():
    listener: Any
    try:
      listener.detach()
    except Exception:
      pass
  CHANGE_LISTENERS.clear()

  for window in sublime.windows():
    window: sublime.Window
    for view in window.views():
      view: sublime.View
      if view.is_valid():
        view.erase_status(STATUS_KEY)
        for region_key in REGION_KEYS:
          region_key: str
          view.erase_regions(region_key)

def _schedule_repaint(view: Optional[sublime.View], delay: int = REPAINT_DELAY, ensure_overlay: bool = False) -> None:
  if not _is_active_view(view):
    return

  change_count : int                                             = view.change_count()
  cached       : Optional[DocumentCache]                         = DOCUMENT_CACHE.get(view.id())
  extent       : Tuple[float, float]                             = view.viewport_extent()
  position     : Tuple[float, float]                             = view.viewport_position()
  visible      : sublime.Region                                  = view.visible_region()
  visible_begin: int                                             = max(0, visible.begin())
  visible_end  : int                                             = min(view.size(), visible.end())
  paint_begin  : int                                             = max(0, min(visible_begin, view.layout_to_text((position[0], max(0, position[1] - extent[1])))))
  paint_end    : int                                             = min(view.size(), max(visible_end, view.layout_to_text((position[0] + extent[0], position[1] + 2 * extent[1]))))
  layout       : Tuple[Tuple[float, float], Tuple[float, float]] = (position, extent)
  revision     : Tuple[Any, ...]                                 = (change_count, visible_begin, visible_end, paint_begin, paint_end, view.settings().get("syntax"), position, extent)

  if paint_end > BUFFER_CHARACTER_LIMIT:
    DOCUMENT_CACHE.pop(view.id(), None)
    DOCUMENT_VIEWPORT_CACHE.pop(view.id(), None)
    PENDING_CHANGES.pop(view.id(), None)
    REPAINT_JOBS.pop(view.id(), None)
    view.erase_status(STATUS_KEY)
    return

  if ensure_overlay and int(sublime.version()) >= 3150:
    color_scheme: Optional[str] = view.settings().get("color_scheme")
    if color_scheme:
      background: Any = "#000000"
      try:
        background = view.style().get("background", background)
      except Exception:
        pass
      else:
        directory         : str                  = os.path.join(sublime.packages_path(), "User", "Color Schemes", PACKAGE_NAME)
        filename          : str                  = os.path.splitext(color_scheme.replace("\\", "/").rsplit("/", 1)[-1])[0] + ".sublime-color-scheme"
        nearest_background: Any                  = background
        path              : str                  = os.path.join(directory, filename)
        previous_text     : Optional[str]        = None
        rules             : List[Dict[str, str]] = []

        if isinstance(background, str) and len(background) == 7 and background.startswith("#"):
          try:
            value: int = int(background[1:], 16)
            red  : int = (value >> 16) & 0xFF
            nearest_background = "#" + hex(red + 1 if red < 255 else red - 1)[2:].upper().zfill(2) + background[3:]
          except ValueError:
            nearest_background = "#000001"
        elif not isinstance(background, str):
          nearest_background = "#000001"

        for depth, foreground in enumerate(COLORS):
          depth     : int
          foreground: str
          rules.append({"background": nearest_background, "foreground": foreground, "name": "Polychrome depth " + str(depth + 1), "scope": POLYCHROME_SCOPES[depth]})

        content: str = json.dumps({"name": "Polychrome generated overlay", "rules": rules}, indent=2, sort_keys=True) + "\n"
        try:
          with io.open(path, "r", encoding="utf-8") as source_file:
            previous_text = source_file.read()
        except (IOError, OSError):
          pass
        if previous_text != content:
          try:
            if not os.path.isdir(directory):
              os.makedirs(directory)
            with io.open(path, "w", encoding="utf-8") as destination_file:
              destination_file.write(content)
            delay = max(delay, 120)
          except (IOError, OSError):
            pass

  if int(sublime.version()) >= 4081 and hasattr(sublime_plugin, "TextChangeListener") and view.buffer_id() not in CHANGE_LISTENERS:
    try:
      listener: PolychromeTextChangeListener = PolychromeTextChangeListener()
      listener.attach(view.buffer())
      CHANGE_LISTENERS[view.buffer_id()] = listener
    except Exception:
      pass

  if cached is not None and cached.ready and cached.revision == change_count and cached.syntax == revision[5] and cached.viewport is not None and (cached.viewport.begin, cached.viewport.end) == (visible_begin, visible_end) and cached.paint_window == (paint_begin, paint_end) and cached.viewport.layout == layout and (cached.points.typecode == "Q" or view.size() <= POSITION_LIMIT):
    return

  if view.id() in REPAINT_JOBS and REPAINT_JOBS[view.id()][:8] == revision:
    return

  repaint: Generator[int, None, None] = _repaint(view, change_count, visible_begin, visible_end, paint_begin, paint_end, layout)
  REPAINT_JOBS[view.id()] = revision + (repaint,)
  _set_status(view, "Queued", 1)

  def step() -> None:
    if REPAINT_JOBS.get(view.id()) != revision + (repaint,):
      return
    if not _is_active_view(view) or view.change_count() != change_count:
      REPAINT_JOBS.pop(view.id(), None)
      return
    try:
      wait: int = next(repaint)
    except StopIteration:
      REPAINT_JOBS.pop(view.id(), None)
      incomplete: Optional[DocumentCache] = DOCUMENT_CACHE.get(view.id())
      if incomplete is not None and not incomplete.ready:
        DOCUMENT_CACHE.pop(view.id(), None)
        DOCUMENT_VIEWPORT_CACHE.pop(view.id(), None)
        PENDING_CHANGES.pop(view.id(), None)
      return
    sublime.set_timeout_async(step, wait)

  sublime.set_timeout_async(step, delay)

def _set_status(view: sublime.View, action: str, progress: int, processed: Optional[int] = None, total: Optional[int] = None) -> None:
  # ->> Progress sizes describe the three-page paint window, never the whole file.
  # ->> The total is measured in UTF-8 bytes without reading beyond that window.
  cache    : Optional[DocumentCache] = DOCUMENT_CACHE.get(view.id())
  formatted: List[str]               = []
  status   : str
  units    : Tuple[str, ...]         = ("bytes", "KB", "MB", "GB", "TB")

  if total is None:
    total = cache.status_bytes if cache is not None else 0
  if processed is None:
    processed = total * max(0, min(progress, 100)) // 100

  for amount in (max(0, min(processed, total)), max(0, total)):
    amount: int
    scaled: float = float(amount)
    unit  : str   = units[0]
    for candidate in units[1:]:
      candidate: str
      if scaled < 1024.0:
        break
      scaled /= 1024.0
      unit    = candidate
    formatted.append((str(int(scaled)) if unit == "bytes" else ("%.1f" % scaled).rstrip("0").rstrip(".")) + " " + unit)

  status = PACKAGE_NAME + ": " + action + " (" + str(progress) + "% · " + formatted[0] + " / " + formatted[1] + ")"
  if view.get_status(STATUS_KEY) != status:
    view.set_status(STATUS_KEY, status)

# Main
def plugin_loaded() -> None:
  global VIEWPORT_POLLING

  # ->> Generate rather than bundle the Command Palette entry.
  path   : str           = os.path.join(sublime.packages_path(), "User", "Polychrome.sublime-commands")
  content: str           = json.dumps([{"caption": "Polychrome: Repaint", "command": "polychrome_repaint"}], indent=2, sort_keys=True) + "\n"
  current: Optional[str] = None
  try:
    with io.open(path, "r", encoding="utf-8") as source_file:
      current = source_file.read()
  except (IOError, OSError):
    pass
  if current != content:
    try:
      if not os.path.isdir(os.path.dirname(path)):
        os.makedirs(os.path.dirname(path))
      with io.open(path, "w", encoding="utf-8") as destination_file:
        destination_file.write(content)
    except (IOError, OSError):
      pass

  for window in sublime.windows():
    window: sublime.Window
    for group in range(window.num_groups()):
      group: int
      _schedule_repaint(window.active_view_in_group(group), 0, True)

  if not VIEWPORT_POLLING:
    VIEWPORT_POLLING = True
    sublime.set_timeout_async(_poll_viewports, VIEWPORT_POLL_DELAY)

def plugin_unloaded() -> None:
  global VIEWPORT_POLLING
  VIEWPORT_POLLING = False
  _reset_all()
