public static class Program {
  public class Alias<T> : System.Dynamic.DynamicObject /* ->> See `https://learn.microsoft.com/en-us/dotnet/api/system.dynamic.dynamicobject?view=net-9.0` */ {
    public T Value;

    public Alias()           => this.Value = default!;
    public Alias(in T value) => this.Value = value;

    public override bool TryGetMember(System.Dynamic.GetMemberBinder binder, out object result) {
      System.Reflection.FieldInfo field = typeof(T).GetField(binder.Name);

      // …
      if (field is not null) {
        result = field.GetValue(this.Value);
        return true;
      }

      result = default!;
      return false;
    }

    public static implicit operator T(in Alias<T> alias) => alias.Value;
  }

  public class A {
    public int n;
  }

  public static void foo(A? _) {}

  /* Main */
  static void Main(string[] arguments) {
    foo(new Alias<A>());
    dynamic alias = new Alias<A>(new() {n = 42});
    System.Console.WriteLine(alias.n);
  }
}
