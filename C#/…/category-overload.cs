public static class Program {
  public class  Class  {}
  public struct Struct {}

  public static void foo<T>(object? _ = null) where T : unmanaged => System.Console.WriteLine("value");
  public static void foo<T>(string? _ = null) where T : class?    => System.Console.WriteLine("reference");

  /* Main */
  static void Main(string[] arguments) {
    foo<int>   ();
    foo<Struct>();

    foo<int?>(); // 🚫 error

    foo<object>();
    foo<Class> ();
  }
}
