#nullable enable annotations

/* ... --- CITE (Lapys) -> https://stackoverflow.com/questions/32025201/how-can-i-determine-if-an-implicit-cast-exists-in-c */
public class Program {
  // --- CITE (Lapys) -> https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/numeric-conversions
  private static System.Collections.Generic.Dictionary<System.Type, System.Type[]> IMPLICIT_CASTS = new() {
    {typeof(byte),   new[] {typeof(decimal), typeof(double), typeof(float), typeof(int), typeof(long), typeof(nint), typeof(nuint), typeof(short), typeof(uint), typeof(ulong), typeof(ushort)}},
    {typeof(float),  new[] {typeof(double)}},
    {typeof(int),    new[] {typeof(decimal), typeof(double), typeof(float), typeof(long), typeof(nint)}},
    {typeof(long),   new[] {typeof(decimal), typeof(double), typeof(float)}},
    {typeof(nint),   new[] {typeof(decimal), typeof(double), typeof(float), typeof(long)}},
    {typeof(nuint),  new[] {typeof(decimal), typeof(double), typeof(float), typeof(ulong)}},
    {typeof(sbyte),  new[] {typeof(decimal), typeof(double), typeof(float), typeof(int), typeof(long), typeof(nint), typeof(short)}},
    {typeof(short),  new[] {typeof(decimal), typeof(double), typeof(float), typeof(int), typeof(long), typeof(nint)}},
    {typeof(uint),   new[] {typeof(decimal), typeof(double), typeof(float), typeof(long), typeof(nuint), typeof(ulong)}},
    {typeof(ulong),  new[] {typeof(decimal), typeof(double), typeof(float)}},
    {typeof(ushort), new[] {typeof(decimal), typeof(double), typeof(float), typeof(int), typeof(long), typeof(nint), typeof(nuint), typeof(uint), typeof(ulong)}}
  };

  /* ... */
  public static bool IsConvertible(System.Type typeA, System.Type typeB) {
    if (null == typeA) return null == typeB;
    if (null == typeB) return null == typeA;

    for (System.Collections.Generic.List<System.Type> types = new() {typeB}; 0 != types.Count; types.RemoveAt(0)) {
      System.Type type = types[0];

      // ...
      if (
        type               == typeA  ||
        type.IsAssignableFrom(typeA) ||
        System.Array.Exists(new[] {type, typeA}, _ => System.Array.Exists(_.GetMethods(System.Reflection.BindingFlags.Public | System.Reflection.BindingFlags.Static), method => {
          System.Reflection.ParameterInfo[] parameters = method.GetParameters();
          System.Reflection.ParameterInfo?  parameter  = 0 != parameters.Length ? parameters[0] : null;

          return method.Name == "op_Implicit"
            && (method    .ReturnType    == type  || IsImplicitConvertible(typeA, type))
            && (parameter?.ParameterType == typeA || IsImplicitConvertible(typeA, parameter?.ParameterType));
        }))
      ) return true;

      // ...
      if (IMPLICIT_CASTS.TryGetValue(typeA, out System.Type[] implicitTypes))
      if (System.Array.IndexOf(implicitTypes, typeB) != implicitTypes.GetLowerBound(0) - 1) {
        foreach (System.Type implicitType in implicitTypes) {
          if (!types.Contains(implicitType))
          types.Add(implicitType);
        }
      }

      foreach (var GetUnderlyingType in new System.Func<System.Type, System.Type>?[] {
        type.IsEnum ? System.Enum    .GetUnderlyingType : _ => null,
        true        ? System.Nullable.GetUnderlyingType : _ => null
      }) {
        System.Type underlyingType = GetUnderlyingType(type);

        if (!types.Contains(underlyingType) && null != underlyingType)
        types.Add(underlyingType);
      }
    }

    return false;
  }

  // ...
  public static bool IsImplicitConvertible(System.Type typeA, System.Type typeB) {
    if (IMPLICIT_CASTS.TryGetValue(typeA, out System.Type[] implicitTypes))
      return System.Array.IndexOf(implicitTypes, typeB) != implicitTypes.GetLowerBound(0) - 1;

    return false;
  }

  /* Main */
  struct A { public static A[] all = null; /* public int this[string i] { get => 42; } */ };
  struct B { public static implicit operator A[](B _) { return new A[] {new()}; } };
  struct C { public void c() => System.Array.IndexOf(new int[] {1}, 1); };

  static void Main(System.String[] arguments) {
    B b = new();

    System.Console.WriteLine($"[..]: {b.GetType() == typeof(A[])}");
    System.Console.WriteLine($"[..]: {IsConvertible(b.GetType(), typeof(A[]))}");
    System.Console.WriteLine($"[..]: {IsConvertible(typeof(A[]), b.GetType())}");
    System.Console.WriteLine($"");
    System.Console.WriteLine($"[..]: {b.GetType() == typeof(A?[])}");
    System.Console.WriteLine($"[..]: {IsConvertible(b.GetType(), typeof(A?[]))}");
    System.Console.WriteLine($"[..]: {IsConvertible(typeof(A?[]), b.GetType())}");
  }
}
