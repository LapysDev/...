#nullable enable

public unsafe static class Program {
  public unsafe static void* AddressOf(System.Delegate function) {
    System.Type                          returnType      = function.Method.ReturnType;
    System.Type[]                        parameterTypes  = System.Array.ConvertAll(function.Method.GetParameters(), static parameter => parameter.ParameterType);
    System.Reflection.Emit.DynamicMethod method          = new("", returnType, parameterTypes, typeof(Program).Module, true);
    System.Reflection.Emit.ILGenerator   methodGenerator = method.GetILGenerator();
    System.Type[]                        types           = new System.Type[parameterTypes.Length + 1];

    // …
    System.Array.Copy(parameterTypes, 0, types, 0, parameterTypes.Length);
    types[parameterTypes.Length] = returnType;

    for (int index = 0; index != parameterTypes.Length; ++index)
      methodGenerator.Emit(System.Reflection.Emit.OpCodes.Ldarg, index);

    methodGenerator.EmitCall(function.Method.IsStatic ? System.Reflection.Emit.OpCodes.Call : System.Reflection.Emit.OpCodes.Callvirt, function.Method, null);
    methodGenerator.Emit    (System.Reflection.Emit.OpCodes.Ret);

    return System.Runtime.InteropServices.Marshal.GetFunctionPointerForDelegate(method.CreateDelegate(System.Linq.Expressions.Expression.GetDelegateType(types))).ToPointer();
  }

  /* Main --> cls && dotnet run --configuration Release & dotnet clean --configuration Release */
  static void Main(string[] arguments) {
    delegate* unmanaged<in int, void> function = (delegate* unmanaged<in int, void>) AddressOf(static (in int x) => {});
    function(42);
  }
}
