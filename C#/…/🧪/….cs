public class A {
  public int n;
  public override string? ToString() => this.n.ToString();
  public static implicit operator A(int _) => new() {n = _};
}
public class B : A {}
public class C : B {}
public class D : A {}
public class E : A {}
public class F : E {}
public class G : F {}

public static class Program {
  public static System.Array bar(System.Runtime.CompilerServices.ITuple tuple) {
    int length = tuple?.Length ?? 0;

    // …
    if (0 != length) {
      System.Type                                                                                                            commonType      = typeof(object);
      ((int size, System.Type? type) floating, (int size, System.Type? type) signed, (int size, System.Type? type) unsigned) promoted        = ((0, null), (0, null), (0, null));
      System.Collections.Generic.Stack<System.Type>[]                                                                        typeHierarchies = new System.Collections.Generic.Stack<System.Type>[length];

      // …
      for (int index = length; 0 != index--; ) {
        System.Type                                   type          = tuple![index]?.GetType() ?? typeof(object);
        System.Collections.Generic.Stack<System.Type> typeHierarchy = new();

        // …
        unsafe {
          if (promoted.floating.size != -1) {
            if      (type == typeof(float))   promoted.floating = promoted.floating.size <= sizeof(float)   ? promoted.floating.size != sizeof(decimal)                                ? (sizeof(float),   typeof(float))   : (-1, null) : promoted.floating;
            else if (type == typeof(double))  promoted.floating = promoted.floating.size <= sizeof(double)  ? promoted.floating.size != sizeof(decimal)                                ? (sizeof(double),  typeof(double))  : (-1, null) : promoted.floating;
            else if (type == typeof(decimal)) promoted.floating = promoted.floating.size <= sizeof(decimal) ? promoted.floating.size == sizeof(decimal) || 0 == promoted.floating.size ? (sizeof(decimal), typeof(decimal)) : (-1, null) : promoted.floating;
            else                              promoted.floating = (-1, null);
          }

          if (promoted.signed.size != -1) {
            if      (type == typeof(sbyte)) promoted.signed = promoted.signed.size == 0                                                                                                  ? (sizeof(sbyte), typeof(sbyte)) : promoted.signed.size == sizeof(ushort) ? (sizeof(int), typeof(int)) : promoted.signed;
            else if (type == typeof(short)) promoted.signed = promoted.signed.size == 0                                                                                                  ? (sizeof(short), typeof(short)) : promoted.signed.size == sizeof(sbyte)  ? (sizeof(int), typeof(int)) : promoted.signed;
            else if (type == typeof(int))   promoted.signed = promoted.signed.size <= sizeof(int)                                                                                        ? (sizeof(int),   typeof(int))                                                                         : promoted.signed;
            else if (type == typeof(long))  promoted.signed = promoted.signed.size <= sizeof(long)                                                                                       ? (sizeof(long),  typeof(long))                                                                        : promoted.signed;
            else if (type == typeof(nint))  promoted.signed = promoted.signed.size <= sizeof(nint) && promoted.signed.type != (sizeof(int) == sizeof(nint) ? typeof(int) : typeof(long)) ? (sizeof(nint),  typeof(nint))                                                                        : promoted.signed;
            else                            promoted.signed = (-1, null);
          }

          if (promoted.unsigned.size != -1) {
            if      (type == typeof(byte))   promoted.unsigned = promoted.unsigned.size == 0                                                                                                         ? (sizeof(byte),   typeof(byte))   : promoted.unsigned.size == sizeof(ushort) ? (sizeof(uint), typeof(uint)) : promoted.unsigned;
            else if (type == typeof(ushort)) promoted.unsigned = promoted.unsigned.size == 0                                                                                                         ? (sizeof(ushort), typeof(ushort)) : promoted.unsigned.size == sizeof(byte)   ? (sizeof(uint), typeof(uint)) : promoted.unsigned;
            else if (type == typeof(uint))   promoted.unsigned = promoted.unsigned.size <= sizeof(uint)                                                                                              ? (sizeof(uint),   typeof(uint))                                                                             : promoted.unsigned;
            else if (type == typeof(ulong))  promoted.unsigned = promoted.unsigned.size <= sizeof(ulong)                                                                                             ? (sizeof(ulong),  typeof(ulong))                                                                            : promoted.unsigned;
            else if (type == typeof(nuint))  promoted.unsigned = promoted.unsigned.size <= sizeof(nuint) && promoted.unsigned.type != (sizeof(nuint) == sizeof(uint) ? typeof(uint) : typeof(ulong)) ? (sizeof(nuint),  typeof(nuint))                                                                            : promoted.unsigned;
            else                             promoted.unsigned = (-1, null);
          }
        }

        for (; type is not null; type = type.BaseType)
          typeHierarchy.Push(type);

        typeHierarchies[index] = typeHierarchy;
      }

      for (bool common = true; common && 0 != typeHierarchies[0].Count; ) {
        System.Type type = typeHierarchies[0].Pop();

        // …
        for (int index = length; 0 != --index; )
        if (0 == typeHierarchies[index].Count || type != typeHierarchies[index].Pop()) {
          common = false;
          break;
        }

        if (common)
        commonType = type;
      }

      System.Console.WriteLine($"[#]: {commonType} {promoted.floating}");
      // System.Array.CreateInstance(? : typeof(object), tuple.Length)
    }

    return System.Array.Empty<object>();
  }

  public static System.Runtime.CompilerServices.ITuple foo(System.Array array) {
    uint length = (uint) (array?.Length ?? 0);

    // …
    if (0u != length && array!.GetType().GetElementType() is System.Type type) {
      object      tuple       = array.GetValue(length - 1u)!;
      uint        tupleCount  = (length / 7u) * 7u;
      System.Type tupleType   = type;
      bool        unmaximized = length != 7u;

      // …
      if (length != tupleCount) {
        uint          count        = length - tupleCount;
        object[]      arguments    = new object[count];
        System.Type[] genericTypes = new System.Type[count];

        // …
        System.Array.Copy(array, length -= count, arguments, 0u, count);
        genericTypes = System.Array.ConvertAll(new System.Type[count], _ => type); // --> System.Array.Fill(…);

        tupleType = (count switch {
          1u => typeof(System.ValueTuple<>),
          2u => typeof(System.ValueTuple<,>),
          3u => typeof(System.ValueTuple<,,>),
          4u => typeof(System.ValueTuple<,,,>),
          5u => typeof(System.ValueTuple<,,,,>),
          6u => typeof(System.ValueTuple<,,,,,>),
          _  => System.Type.GetType("System.ValueTuple`" + count)
        }).MakeGenericType(genericTypes);
        tuple = System.Activator.CreateInstance(tupleType, arguments);
      }

      if (0u != tupleCount) {
        object[]      arguments    = new object[7u + (unmaximized ? 1u : 0u)];
        uint          count        = 6u + (unmaximized ? 1u : 0u);
        System.Type[] genericTypes = new System.Type[7u + (unmaximized ? 1u : 0u)];
        System.Type   subtupleType = unmaximized ? typeof(System.ValueTuple<,,,,,,,>) : typeof(System.ValueTuple<,,,,,,>);

        // …
        for (genericTypes = /* --> System.Array.Fill(…); */ System.Array.ConvertAll(genericTypes, _ => type); 0u != tupleCount; tupleCount -= 7u) {
          System.Array.Copy(array, length -= 7u, arguments, 0u, 7u);

          arguments   [count] = tuple;
          genericTypes[count] = tupleType;
          tupleType           = subtupleType.MakeGenericType(genericTypes);
          tuple               = System.Activator.CreateInstance(tupleType, arguments);
        }
      }

      return (System.Runtime.CompilerServices.ITuple) tuple;
    }

    return System.ValueTuple.Create();
  }

  /* Main --> del ….exe ….rsp & cls && (for %I in (*.dll) do @echo /r:"%I") > ….rsp && csc /langversion:preview /nullable /out:….exe @….rsp /t:exe /unsafe /warnaserror ….cs && ….exe & del ….exe ….rsp */
  static void Main(string[] arguments) {
    // bar((new C(), new D(), new G()));
    bar(((float) 0, (decimal) 1, (float) 2));
    // System.Console.WriteLine((new A() {n=1}, new A() {n=2}, new A() {n=3}));
    // System.Console.WriteLine((new A() {n=1}, new A() {n=2}, new A() {n=3}, new A() {n=4}, new A() {n=5}, new A() {n=6}, new A() {n=7}));
    // System.Console.WriteLine((new A() {n=1}, new A() {n=2}, new A() {n=3}, new A() {n=4}, new A() {n=5}, new A() {n=6}, new A() {n=7}, new A() {n=8}, new A() {n=9}, new A() {n=10}, new A() {n=11}, new A() {n=12}, new A() {n=13}, new A() {n=14}, new A() {n=15}, new A() {n=16}, new A() {n=17}, new A() {n=18}, new A() {n=19}, new A() {n=20}, new A() {n=21}, new A() {n=22}, new A() {n=23}, new A() {n=24}, new A() {n=25}, new A() {n=26}, new A() {n=27}, new A() {n=28}, new A() {n=29}, new A() {n=30}, new A() {n=31}, new A() {n=32}, new A() {n=33}, new A() {n=34}, new A() {n=35}, new A() {n=36}));
    // System.Console.Write("T[0]");  System.Console.WriteLine(foo(new A[0]));
    // System.Console.Write("T[1]");  System.Console.WriteLine(foo(new A[1]  {1}));
    // System.Console.Write("T[3]");  System.Console.WriteLine(foo(new A[3]  {1, 2, 3}));
    // System.Console.Write("T[7]");  System.Console.WriteLine(foo(new A[7]  {1, 2, 3, 4, 5, 6, 7}));
    // System.Console.Write("T[36]"); System.Console.WriteLine(foo(new A[36] {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36}));
  }
}
