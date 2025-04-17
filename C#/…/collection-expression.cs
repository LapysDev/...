namespace System.Runtime.CompilerServices {
  [System.AttributeUsage(System.AttributeTargets.Class | System.AttributeTargets.Interface | System.AttributeTargets.Struct, Inherited = false)]
  [System.Diagnostics.DebuggerNonUserCode]
  [System.Diagnostics.CodeAnalysis.ExcludeFromCodeCoverage]
  internal sealed class CollectionBuilderAttribute : System.Attribute {
    public System.Type BuilderType { get; }
    public string      MethodName  { get; }

    /* … */
    public CollectionBuilderAttribute(System.Type builderType, string methodName) {
      this.BuilderType = builderType;
      this.MethodName  = methodName;
    }
  }
}
