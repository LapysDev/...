#nullable enable annotations

public static class Program {
  public const double a = double.PositiveInfinity;

  /* Main --> del ….exe ….rsp & cls && (for %I in (*.dll) do @echo /r:"%I") > ….rsp && csc /langversion:preview /out:….exe @….rsp /t:exe /unsafe /warnaserror ../required.cs ….cs && ….exe & del ….exe ….rsp */
  static void Main(string[] arguments) {}
}
