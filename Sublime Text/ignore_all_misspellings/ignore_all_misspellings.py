# %AppData%/Sublime Text/Packages/User
import sublime_plugin

# ...
class IgnoreAllMisspellings(sublime_plugin.TextCommand):
  def run(self, edit):
    view        = self.view
    substr      = view.substr
    selections  = view.sel()
    seen        = set()
    run_command = view.run_command
    regions     = []

    while True:
      run_command("next_misspelling")

      region   = selections[0]
      position = (region.a, region.b)

      if position in seen:
        break

      seen.add(position)
      regions.append(region)

      for word in {substr(region) for region in selections}:
        run_command("ignore_word", {"word": word})

    selections.clear()
    selections.add_all(regions)
