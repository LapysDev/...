namespace System.Diagnostics.CodeAnalysis {
  [System.AttributeUsage(System.AttributeTargets.Constructor, AllowMultiple = false, Inherited = false)]
  public sealed class SetsRequiredMembersAttribute : System.Attribute {}
}

namespace System.Runtime.CompilerServices {
  public class CompilerFeatureRequiredAttribute : System.Attribute {
    public CompilerFeatureRequiredAttribute(string name) {}
  }

  public class RequiredMemberAttribute : System.Attribute {}
}
