namespace System.Runtime.CompilerServices {
  [System.AttributeUsage(System.AttributeTargets.Class | System.AttributeTargets.Interface | System.AttributeTargets.Struct, Inherited = false)]
  [System.Diagnostics.DebuggerNonUserCode]
  [System.Diagnostics.CodeAnalysis.ExcludeFromCodeCoverage]
  internal sealed class CollectionBuilderAttribute : System.Attribute {
    public System.Type BuilderType { get; }
    public string      MethodName  { get; }

    public CollectionBuilderAttribute(System.Type builderType, string methodName) {
      this.BuilderType = builderType;
      this.MethodName  = methodName;
    }
  }
}

/* … */
public static class Program {
  [UnityEngine.RuntimeInitializeOnLoadMethod]
  private static void Main() {
    new UnityEngine.GameObject("…", typeof(ProgramBehaviour));
  }
}

internal sealed class ProgramBehaviour : UnityEngine.MonoBehaviour {
  public struct B {
    public int this[int index] => 42;
    public int this[System.Range _] => 42;
  }

  private static void Awake() {
    System.Console.WriteLine(new B()[0..1]);
     System.Collections.Generic.List<int> list = [1, 2, 3];
  }

  private static void FixedUpdate() {}
  private static void OnDestroy  () {}
}
