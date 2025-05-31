public static class Program {
  public static dynamic foo() => (object) "Hello";
  public static void bar(string _) {}

  /* Main */
  static void Main(string[] arguments) {
    System.Console.WriteLine(((object) "Hello").Length); // 🚫
    System.Console.WriteLine(foo().Length);              // ✅
    bar(foo());
  }
}
