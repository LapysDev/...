public static class Program {
  public struct A {}

  /* Main */
  static void Main(string[] arguments) {
    System.Console.WriteLine(object                                                .Equals         (new A(), new A())); // ⟶ `virtual bool Equals(…)` member-wise reflective compare
    System.Console.WriteLine(object                                                .ReferenceEquals(new A(), new A())); // ⟶ `false`
    System.Console.WriteLine(System.Collections.Generic.EqualityComparer<A>.Default.Equals         (new A(), new A())); // ⟶ `(… as IEquatable<T>)?.Equals(…) ?? object.Equals(…, …)` (de-virtualized/ inlined)
    System.Console.WriteLine(System.Runtime.CompilerServices.RuntimeHelpers        .Equals         (new A(), new A())); // ⟶ `memcmp(…)` (not alignment-/ offset-/ padding-agnostic)
  }
}
