# %AppData%/Sublime Text/Packages/User
import sublime
import sublime_plugin

# ...
class AddAllMisspellings(sublime_plugin.TextCommand):
  def run(self, edit):
    selections          = self.view.sel()
    original_selections = list(selections)
    selected_ranges     = [region for region in original_selections if not region.empty()]
    seen                = set()
    words               = set()

    try:
      selections.clear()
      selections.add(sublime.Region(0))

      while True:
        self.view.run_command("next_misspelling")

        region   = selections[0]
        position = (region.begin(), region.end())

        if region.empty() or position in seen:
          break

        seen.add(position)

        if not selected_ranges or any(selected.begin() <= region.begin() and region.end() <= selected.end() for selected in selected_ranges):
          words.add(self.view.substr(region))

      for word in sorted(words):
        self.view.run_command("add_word", {"word": word})
    finally:
      selections.clear()
      selections.add_all(original_selections)
