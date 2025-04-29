public static class Program {
  public struct A { public int _; }
  public struct B { public int a; public int b; }

  public unsafe static A Foo<T>(object? _ = null) where T : unmanaged => new();
  public unsafe static B Foo<T>(string? _ = null) where T : class? => new();

  public class Class {}
  public struct Struct {}

  /* Main */
  static void Main(string[] arguments) {
    Bar<Class>();
    Bar<object>();
    Bar<Struct>();
    Bar<int>();
  }
}
