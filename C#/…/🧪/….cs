public static class Program {
  class A {}
  /* Main --> del ….exe ….rsp & cls && (for %I in (*.dll) do @echo /r:"%I") > ….rsp && csc /langversion:preview /nullable /out:….exe @….rsp /t:exe /unsafe /warnaserror ….cs && ….exe & del ….exe ….rsp */
  static void Main(string[] arguments) {
    System.Span<A> span = [new A()];
  }
}
