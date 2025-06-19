#nullable enable

public static class Program {
  public readonly struct Void { /* ->> “error CS0590: UsEr-DeFiNeD oPeRaToRs CaNnOt ReTuRn VoId” */ }

  public delegate V    BinaryOperation      <T, U, V>(in  T valueA, in U valueB);
  public delegate void BinaryAssignOperation<T, U>   (ref T value,  in U operand);

  private static System.Collections.Generic.Dictionary<(System.Type, System.Type, System.Type, string, bool), System.Reflection.Emit.DynamicMethod> BinaryDelegates = new();

  private unsafe static System.Reflection.Emit.DynamicMethod GetBinaryDelegate<T, U, V>(string name, in System.ReadOnlySpan<System.Reflection.Emit.OpCode> operations, bool assign = false) {
    if (!BinaryDelegates.TryGetValue((typeof(T), typeof(U), typeof(V), name, assign), out System.Reflection.Emit.DynamicMethod? method))
    fixed (System.Reflection.Emit.OpCode* address = operations) {
      System.Reflection.Emit.ILGenerator generator;

      // ...
      method    = new($"_{name ?? (operations.Length == 1 ? address -> ToString() : "Binary")}{typeof(T).Name}{typeof(U).Name}", assign ? typeof(void) : typeof(V), new[] {typeof(T).MakeByRefType(), typeof(U).MakeByRefType()}, typeof(Program).Module, true);
      generator = method.GetILGenerator();

      generator.Emit(System.Reflection.Emit.OpCodes.Ldarg_0);
      if (assign) { generator.Emit(System.Reflection.Emit.OpCodes.Ldarg_0); }
      generator.Emit(System.Reflection.Emit.OpCodes.Ldobj, typeof(T)); // --> `in T`
      generator.Emit(System.Reflection.Emit.OpCodes.Ldarg_1);
      generator.Emit(System.Reflection.Emit.OpCodes.Ldobj, typeof(U)); // --> `in U`
      for (System.Reflection.Emit.OpCode* iterator = address, end = address + operations.Length; end != iterator; ++iterator) { generator.Emit(*iterator); }
      if (assign)                                                                                                             { generator.Emit(System.Reflection.Emit.OpCodes.Stobj, typeof(T)); }
      generator.Emit(System.Reflection.Emit.OpCodes.Ret);

      BinaryDelegates.Add((typeof(T), typeof(U), typeof(V), name!, assign), method);
    }

    return method!;
  }

  public unsafe static BinaryOperation      <T, U, V> GetAdditionDelegate          <T, U, V>(string name = null!)                                                               =>                               GetBinaryDelegate      <T, U, V>   (name ?? "Addition",           System.Reflection.Emit.OpCodes.Add);
  public unsafe static BinaryAssignOperation<T, U>    GetAdditionAssignDelegate    <T, U>   (string name = null!)                                                               =>                               GetBinaryAssignDelegate<T, U>      (name ?? "AdditionAssign",     System.Reflection.Emit.OpCodes.Add);
  public unsafe static BinaryOperation      <T, U, V> GetBinaryDelegate            <T, U, V>             (params System.ReadOnlySpan<System.Reflection.Emit.OpCode> operations) =>                               GetBinaryDelegate      <T, U, V>   ((null as string)!,            operations);
  public unsafe static BinaryOperation      <T, U, V> GetBinaryDelegate            <T, U, V>(string name, params System.ReadOnlySpan<System.Reflection.Emit.OpCode> operations) => (BinaryOperation<T, U, V>)    GetBinaryDelegate      <T, U, V>   (name,                      in operations, assign: false).CreateDelegate(typeof(BinaryOperation<T, U, V>));
  public unsafe static BinaryAssignOperation<T, U>    GetBinaryAssignDelegate      <T, U>                (params System.ReadOnlySpan<System.Reflection.Emit.OpCode> operations) =>                               GetBinaryAssignDelegate<T, U>      ((null as string)!,            operations);
  public unsafe static BinaryAssignOperation<T, U>    GetBinaryAssignDelegate      <T, U>   (string name, params System.ReadOnlySpan<System.Reflection.Emit.OpCode> operations) => (BinaryAssignOperation<T, U>) GetBinaryDelegate      <T, U, Void>(name,                      in operations, assign: true).CreateDelegate(typeof(BinaryAssignOperation<T, U>));
  public unsafe static BinaryOperation      <T, U, V> GetBitwiseAndDelegate        <T, U, V>(string name = null!)                                                               =>                               GetBinaryDelegate      <T, U, V>   (name ?? "BitwiseAnd",         System.Reflection.Emit.OpCodes.And);
  public unsafe static BinaryAssignOperation<T, U>    GetBitwiseAndAssignDelegate  <T, U>   (string name = null!)                                                               =>                               GetBinaryAssignDelegate<T, U>      (name ?? "BitwiseAndAssign",   System.Reflection.Emit.OpCodes.And);
  public unsafe static BinaryOperation      <T, U, V> GetBitwiseOrDelegate         <T, U, V>(string name = null!)                                                               =>                               GetBinaryDelegate      <T, U, V>   (name ?? "BitwiseOr",          System.Reflection.Emit.OpCodes.Or);
  public unsafe static BinaryAssignOperation<T, U>    GetBitwiseOrAssignDelegate   <T, U>   (string name = null!)                                                               =>                               GetBinaryAssignDelegate<T, U>      (name ?? "BitwiseOrAssign",    System.Reflection.Emit.OpCodes.Or);
  public unsafe static BinaryOperation      <T, U, V> GetDivisionDelegate          <T, U, V>(string name = null!)                                                               =>                               GetBinaryDelegate      <T, U, V>   (name ?? "Division",           System.Reflection.Emit.OpCodes.Div);
  public unsafe static BinaryAssignOperation<T, U>    GetDivisionAssignDelegate    <T, U>   (string name = null!)                                                               =>                               GetBinaryAssignDelegate<T, U>      (name ?? "DivisionAssign",     System.Reflection.Emit.OpCodes.Div);
  public unsafe static BinaryOperation      <T, U, V> GetExclusiveOrDelegate       <T, U, V>(string name = null!)                                                               =>                               GetBinaryDelegate      <T, U, V>   (name ?? "ExclusiveOr",        System.Reflection.Emit.OpCodes.Xor);
  public unsafe static BinaryAssignOperation<T, U>    GetExclusiveOrAssignDelegate <T, U>   (string name = null!)                                                               =>                               GetBinaryAssignDelegate<T, U>      (name ?? "ExclusiveOrAssign",  System.Reflection.Emit.OpCodes.Xor);
  public unsafe static BinaryOperation      <T, U, V> GetGreaterThanDelegate       <T, U, V>(string name = null!)                                                               =>                               GetBinaryDelegate      <T, U, V>   (name ?? "GreaterThan",        System.Reflection.Emit.OpCodes.Cgt);
  public unsafe static BinaryOperation      <T, U, V> GetGreaterThanOrEqualDelegate<T, U, V>(string name = null!)                                                               =>                               GetBinaryDelegate      <T, U, V>   (name ?? "GreaterThanOrEqual", System.Reflection.Emit.OpCodes.Clt, System.Reflection.Emit.OpCodes.Ldc_I4_0, System.Reflection.Emit.OpCodes.Ceq);
  public unsafe static BinaryOperation      <T, U, V> GetInequalityDelegate        <T, U, V>(string name = null!)                                                               =>                               GetBinaryDelegate      <T, U, V>   (name ?? "Inequality",         System.Reflection.Emit.OpCodes.Ceq, System.Reflection.Emit.OpCodes.Ldc_I4_0, System.Reflection.Emit.OpCodes.Ceq);
  public unsafe static BinaryOperation      <T, U, V> GetLeftShiftDelegate         <T, U, V>(string name = null!)                                                               =>                               GetBinaryDelegate      <T, U, V>   (name ?? "LeftShift",          System.Reflection.Emit.OpCodes.Shl);
  public unsafe static BinaryAssignOperation<T, U>    GetLeftShiftAssignDelegate   <T, U>   (string name = null!)                                                               =>                               GetBinaryAssignDelegate<T, U>      (name ?? "LeftShiftAssign",    System.Reflection.Emit.OpCodes.Shl);
  public unsafe static BinaryOperation      <T, U, V> GetLessThanDelegate          <T, U, V>(string name = null!)                                                               =>                               GetBinaryDelegate      <T, U, V>   (name ?? "LessThan",           System.Reflection.Emit.OpCodes.Clt);
  public unsafe static BinaryOperation      <T, U, V> GetLessThanOrEqualDelegate   <T, U, V>(string name = null!)                                                               =>                               GetBinaryDelegate      <T, U, V>   (name ?? "LessThanOrEqual",    System.Reflection.Emit.OpCodes.Cgt, System.Reflection.Emit.OpCodes.Ldc_I4_0, System.Reflection.Emit.OpCodes.Ceq);
  public unsafe static BinaryOperation      <T, U, V> GetModulusDelegate           <T, U, V>(string name = null!)                                                               =>                               GetBinaryDelegate      <T, U, V>   (name ?? "Modulus",            System.Reflection.Emit.OpCodes.Rem);
  public unsafe static BinaryAssignOperation<T, U>    GetModulusAssignDelegate     <T, U>   (string name = null!)                                                               =>                               GetBinaryAssignDelegate<T, U>      (name ?? "ModulusAssign",      System.Reflection.Emit.OpCodes.Rem);
  public unsafe static BinaryOperation      <T, U, V> GetMultiplyDelegate          <T, U, V>(string name = null!)                                                               =>                               GetBinaryDelegate      <T, U, V>   (name ?? "Multiply",           System.Reflection.Emit.OpCodes.Mul);
  public unsafe static BinaryAssignOperation<T, U>    GetMultiplyAssignDelegate    <T, U>   (string name = null!)                                                               =>                               GetBinaryAssignDelegate<T, U>      (name ?? "MultiplyAssign",     System.Reflection.Emit.OpCodes.Mul);
  public unsafe static BinaryOperation      <T, U, V> GetRightShiftDelegate        <T, U, V>(string name = null!)                                                               =>                               GetBinaryDelegate      <T, U, V>   (name ?? "RightShift",         System.Reflection.Emit.OpCodes.Shr);
  public unsafe static BinaryAssignOperation<T, U>    GetRightShiftAssignDelegate  <T, U>   (string name = null!)                                                               =>                               GetBinaryAssignDelegate<T, U>      (name ?? "RightShiftAssign",   System.Reflection.Emit.OpCodes.Shr);
  public unsafe static BinaryOperation      <T, U, V> GetSubtractionDelegate       <T, U, V>(string name = null!)                                                               =>                               GetBinaryDelegate      <T, U, V>   (name ?? "Subtraction",        System.Reflection.Emit.OpCodes.Sub);
  public unsafe static BinaryAssignOperation<T, U>    GetSubtractionAssignDelegate <T, U>   (string name = null!)                                                               =>                               GetBinaryAssignDelegate<T, U>      (name ?? "SubtractionAssign",  System.Reflection.Emit.OpCodes.Sub);

  [System.Flags] public enum MyEnum : ulong { ZERO, ONE }
  /* Main --> cls && dotnet run --configuration Release & dotnet clean --configuration Release */
  static void Main(string[] arguments) {
    decimal variable = default!;
    System.Console.WriteLine(variable += default(bool));
    System.Console.WriteLine(variable += default(byte));
    System.Console.WriteLine(variable += default(char));
    System.Console.WriteLine(variable += default(decimal));
    System.Console.WriteLine(variable += default(double));
    System.Console.WriteLine(variable += default(float));
    System.Console.WriteLine(variable += default(int));
    System.Console.WriteLine(variable += default(long));
    System.Console.WriteLine(variable += default(nint));
    System.Console.WriteLine(variable += default(nuint));
    System.Console.WriteLine(variable += default(sbyte));
    System.Console.WriteLine(variable += default(short));
    System.Console.WriteLine(variable += string.Empty);
    System.Console.WriteLine(variable += default(uint));
    System.Console.WriteLine(variable += default(ulong));
    System.Console.WriteLine(variable += default(ushort));
    System.Console.WriteLine(variable += System.DateTime.UnixEpoch);
    System.Console.WriteLine(variable += default(MyEnum));

    // var variable = (byte) 0u;
    // GetSubtractionAssignDelegate<byte, byte>()(ref variable!, (byte) 2u); System.Console.WriteLine(variable);
    // System.Console.WriteLine(GetSubtractionDelegate<byte, byte, byte>()(variable!, (byte) 2u));
    // System.Console.WriteLine(variable);
  }
}
