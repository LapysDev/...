#nullable enable annotations

public static class Program {
  public class Event : System.EventArgs {
    public required object? data  =  null;
    public ref      object? value => ref this.data;
  }

  /* Main --> del ….exe ….rsp & cls && (for %I in (*.dll) do @echo /r:"%I") > ….rsp && csc /out:….exe @….rsp /t:exe ../required.cs ….cs && ….exe & del ….exe ….rsp */
  static void Main(string[] arguments) {
    new Event() {data = null};
  }
}
