public class Program {
  public static void foo<T>(T _) {
    if (new System.Collections.Generic.Dictionary<System.Type, object>() {
      {typeof(int),    (int    _) => System.Console.WriteLine($"int{{{+_}}}")},
      {typeof(string), (string _) => System.Console.WriteLine($"string({_.Length}) {{\"{_}\"}}")}
    }.TryGetValue(typeof(T), out object __)) (__ as System.Action<T>)(_); else System.Console.WriteLine("...{?}");
  }

  /* Main */
  static void Main(string[] arguments) {
    foo(42);
    foo(3.14f);
    foo("Hello, World!");
  }
}
