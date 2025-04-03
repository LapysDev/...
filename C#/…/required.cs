namespace System.Diagnostics.CodeAnalysis /* ⟶ `required` @ `https://github.com/dotnet/core/issues/8016` `https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/keywords/required` */ {
  [System.AttributeUsage(System.AttributeTargets.Constructor, AllowMultiple = false, Inherited = false)]
  public sealed class SetsRequiredMembersAttribute : System.Attribute {}
}

namespace System.Runtime.CompilerServices /* ⟶ `required` @ `https://github.com/dotnet/core/issues/8016` `https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/keywords/required` */ {
  public class CompilerFeatureRequiredAttribute : System.Attribute {
    public CompilerFeatureRequiredAttribute(string name) {}
  }

  public class RequiredMemberAttribute : System.Attribute {}
}
