#nullable enable annotations

public static class Program {
  public static void foo(ref int _) {}

  /* Main --> del ….exe ….rsp & cls && (for %I in (*.dll) do @echo /r:"%I") > ….rsp && csc /langversion:preview /out:….exe @….rsp /t:exe /unsafe /warnaserror ….cs && ….exe & del ….exe ….rsp */
  static void Main(string[] arguments) {
    foo(ref int a);
  }
}
