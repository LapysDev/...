#nullable enable annotations

public static class Program {
  /* Main --> del ….exe ….rsp & cls && (for %I in (*.dll) do @echo /r:"%I") > ….rsp && csc /out:….exe @….rsp /t:exe ../required.cs ….cs && ….exe & del ….exe ….rsp */
  static void Main(string[] arguments) {}
}
