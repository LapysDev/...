public class Program {
  class A<T1, T2> : System.Collections.Generic.Dictionary<T1, T2> {}
  class B<T>      : A<T, int> {}
  class C         : B<string> { public int this[string _] { set { System.Console.WriteLine("[SET]"); } } }

  public static void Foo(int a) { System.Console.WriteLine($"[]: {a}"); }

  /* Main */
  static void Main(System.String[] arguments) {
    System.Reflection.MethodInfo method = typeof(Program).GetMethod("Foo", System.Reflection.BindingFlags.Public | System.Reflection.BindingFlags.Static);
    method?.Invoke(null, new object[] {System.Convert.ChangeType(42, method.GetParameters()[0].ParameterType)});

    for (System.Type type = typeof(C); null != type; type = type.BaseType)
    System.Console.WriteLine($"{type} is {
      type.GetGenericArguments().Length == typeof(System.Collections.Generic.Dictionary<,>).GetGenericArguments().Length &&
      type == typeof(System.Collections.Generic.Dictionary<,>).MakeGenericType(type.GetGenericArguments())
    }");

    new C()[""] = 1;
    typeof(C).GetProperty((System.Attribute.GetCustomAttribute(typeof(C), typeof(System.Reflection.DefaultMemberAttribute)) as System.Reflection.DefaultMemberAttribute)?.MemberName ?? "", new[] {typeof(string)})?.GetSetMethod()?.Invoke(new C(), new object[] {"", 1});
  }
}
