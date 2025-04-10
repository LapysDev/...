#nullable enable annotations

namespace System.Runtime.CompilerServices { internal static class IsExternalInit {} }

public static class Program {
  public class A {
    public string characters = null;
    public ref string text { get { this.characters ??= "Hello"; return ref this.characters; } }
  }

  public class : A {}

  /* Main --> del ….exe ….rsp & cls && (for %I in (*.dll) do @echo /r:"%I") > ….rsp && csc /langversion:preview /out:….exe @….rsp /t:exe /unsafe /warnaserror ../required.cs ….cs && ….exe & del ….exe ….rsp */
  static void Main(string[] arguments) {
    System.Console.WriteLine(new A().characters is null);
    System.Console.WriteLine(new A().text);
  }
}
