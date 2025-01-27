public static class Extensions {
  public static void B(this Program.A _) {
    System.Console.Write("Hello, World!");
  }
}

public class Program {
  public struct A {}

  /* Main */
  static void Main(string[] arguments) {
    new A().B();
  }
}
