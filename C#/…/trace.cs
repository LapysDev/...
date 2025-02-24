public class Program {
  public static void trace(string message, [System.Runtime.CompilerServices.CallerMemberName] string memberName = "", [System.Runtime.CompilerServices.CallerFilePath] string sourceFilePath = "", [System.Runtime.CompilerServices.CallerLineNumber] int sourceLineNumber = 0) {
    System.Console.WriteLine($"[{memberName} @ \"{sourceFilePath}\":{sourceLineNumber}]: {message}");
  }

  /* Main --> del ….exe & cls && csc /out:….exe /t:exe ….cs && ….exe & del ….exe */
  static void Main(string[] arguments) {
    trace("Hello, World!");
  }
}
