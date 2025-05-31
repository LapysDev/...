namespace System.Runtime.Versioning {
  [System.AttributeUsage(System.AttributeTargets.Class | System.AttributeTargets.Constructor | System.AttributeTargets.Method | System.AttributeTargets.Struct, AllowMultiple = false, Inherited = false)]
  internal sealed class NonVersionableAttribute : System.Attribute {
    public NonVersionableAttribute() {}
  }
}

#if !(NET5_0 || NET5_0_OR_GREATER)
  namespace System.Runtime.CompilerServices {
    // ⟶ `init` @ `https://web.archive.org/web/20220918192058/https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/keywords/init`
    internal static class IsExternalInit {}
  }
#endif

#if !(NET7_0 || NET7_0_OR_GREATER)
  namespace System.Runtime.CompilerServices {
    // ⟶ `required` @ `https://web.archive.org/web/20220924164132/https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/keywords/required`
    public        class CompilerFeatureRequiredAttribute : System.Attribute { public CompilerFeatureRequiredAttribute(string name) {} }
    public sealed class RequiredMemberAttribute          : System.Attribute {}
  }

  namespace System.Diagnostics.CodeAnalysis {
    // ⟶ `System.Diagnostics.CodeAnalysis.UnscopedRefAttribute` @ `https://web.archive.org/web/20250401033710/https://learn.microsoft.com/en-us/dotnet/api/system.diagnostics.codeanalysis.unscopedrefattribute?view=net-9.0`
    [System.Diagnostics.CodeAnalysis.ExcludeFromCodeCoverage]
    [System.Diagnostics.DebuggerNonUserCode]
    [System.AttributeUsage(System.AttributeTargets.Method | System.AttributeTargets.Parameter | System.AttributeTargets.Property, Inherited = false)]
    sealed class UnscopedRefAttribute : System.Attribute {}
  }

  // ⟶ `System.Diagnostics.CodeAnalysis.SetsRequiredMembers` @ `https://web.archive.org/web/20250401051947/https://learn.microsoft.com/en-us/dotnet/api/system.diagnostics.codeanalysis.setsrequiredmembersattribute?view=net-9.0`
  [System.AttributeUsage(System.AttributeTargets.Constructor, AllowMultiple = false, Inherited = false)]
  [System.Diagnostics.CodeAnalysis.ExcludeFromCodeCoverage]
  [System.Diagnostics.DebuggerNonUserCode]
  public sealed class PatchConstructor : System.Attribute {}
#endif

#if !(NET8_0 || NET8_0_OR_GREATER)
  namespace System.Runtime.CompilerServices {
    // ⟶ Collection Expressions @ `https://web.archive.org/web/20231114165920/https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/operators/collection-expressions`
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
#endif

#if !(NET9_0 || NET9_0_OR_GREATER)
  // ⟶ `System.Runtime.CompilerServices.OverloadResolutionPriorityAttribute` @ `https://web.archive.org/web/20240920192519/https://learn.microsoft.com/en-us/dotnet/api/system.runtime.compilerservices.overloadresolutionpriorityattribute?view=net-9.0`
  [System.AttributeUsage(System.AttributeTargets.Method | System.AttributeTargets.Constructor | System.AttributeTargets.Property, AllowMultiple = false, Inherited = false)]
  [System.Diagnostics.CodeAnalysis.ExcludeFromCodeCoverage]
  [System.Diagnostics.DebuggerNonUserCode]
  public sealed class PatchResolution : System.Attribute {
    public int Priority { get; }

    /* … */
    public PatchResolution(int priority) => this.Priority = priority;
  }
#endif

#if NET46X || NET47X || NET48X || NETCOREAPP2X || NETSTANDARD2_0
  namespace System {
    // ⟶ `System.Index` @ `https://web.archive.org/web/20241129171203/https://learn.microsoft.com/en-us/dotnet/api/system.index?view=net-9.0`
    [System.Diagnostics.CodeAnalysis.ExcludeFromCodeCoverage]
    [System.Diagnostics.DebuggerNonUserCode]
    public readonly struct Index : System.IEquatable<Index> {
      public  static Index        Start     => new(0);
      public  static Index        End       => new(~0);
      public         bool         IsFromEnd => this.value < 0;
      public         int          Value     => this.value < 0 ? ~this.value : this.value;
      private        readonly int value;

      /* … */
      [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] private Index(int value)                       { this.value = value; }
      [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public  Index(int value, bool fromEnd = false) { if (value < 0) { throw new System.ArgumentOutOfRangeException(nameof(value)); } this.value = fromEnd ? ~value : value; }

      /* … */
      [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public  override bool   Equals                         (object? value)  => value is Index index && this.Equals(index);
      [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public           bool   Equals                         (Index   index)  => index.value == this.value;
      [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public  static   Index  FromEnd                        (int     value)  { if (value < 0) { throw new System.IndexOutOfRangeException(nameof(value)); } return new(~value); }
      [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public  static   Index  FromStart                      (int     value)  { if (value < 0) { throw new System.IndexOutOfRangeException(nameof(value)); } return new(value); }
      [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public  override int    GetHashCode                    ()               => this.value;
      [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public           int    GetOffset                      (int length)     => this.IsFromEnd ? this.value + (length + 1) : this.value;
      [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public  override string ToString                       ()               => this.IsFromEnd ? this.ToStringFromEnd() : ((uint) this.Value).ToString();
      [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] private          string ToStringFromEnd                ()               => $"^{this.Value.ToString()}";
      [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] bool                    System.IEquatable<Index>.Equals(Index index)    => this.Equals(index);

      /* … */
      [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)]
      public static implicit operator Index(int value) => Index.FromStart(value);
    }

    // ⟶ `System.Range` @ `https://web.archive.org/web/20241231015910/https://learn.microsoft.com/en-us/dotnet/api/system.range?view=net-9.0`
    [System.Diagnostics.CodeAnalysis.ExcludeFromCodeCoverage]
    [System.Diagnostics.DebuggerNonUserCode]
    public readonly struct Range : System.IEquatable<Range> {
      public static /* readonly */ Range All => new(System.Index.Start, System.Index.End);

      public readonly System.Index End   { get; } = System.Index.End;
      public readonly System.Index Start { get; } = System.Index.Start;

      /* … */
      [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)]
      public Range(System.Index start, System.Index end) {
        this.End   = end;
        this.Start = start;
      }

      /* … */
      public static Range EndAt(System.Index end) {
        return new(System.Index.Start, end);
      }

      [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public override bool Equals     (object?  value) => value is Range range && this.Equals(range);
      [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public          bool Equals     (in Range range) => this.End.Equals(range.End) && this.Start.Equals(range.Start);
      [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public override int  GetHashCode()               => System.HashCode.Combine(this.End, this.Start);

      [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)]
      public (int Offset, int Length) GetOffsetAndLength(int length) {
        (System.Index endIndex, System.Index startIndex) = (this.End, this.Start);
        (int          end,      int          start)      = (endIndex.IsFromEnd ? length - endIndex.Value : endIndex.Value, startIndex.IsFromEnd ? length - startIndex.Value : startIndex.Value);

        // …
        if ((uint) end > (uint) length || (uint) end < (uint) start)
        throw new System.ArgumentOutOfRangeException(nameof(length));

        return (start, end - start);
      }

      public static Range StartAt(System.Index start) {
        return new(start, System.Index.End);
      }

      [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public override string ToString                       ()            => $"{this.Start}..{this.End}";
      [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] bool                   System.IEquatable<Range>.Equals(Range range) => this.Equals(range);
    }
  }

  namespace System.Runtime.CompilerServices {
    // ⟶ `System.Runtime.CompilerServices.RuntimeHelpers.GetSubArray<…>(…)` @ `https://web.archive.org/web/20250420233858/https://learn.microsoft.com/en-us/dotnet/api/system.runtime.compilerservices.runtimehelpers.getsubarray?view=net-9.0`
    public static partial class RuntimeHelpers {
      public static T[] GetSubArray<T>(T[] array, System.Range range) {
        (int offset, int length) = range.GetOffsetAndLength(array.Length);
        T[] subarray;

        // …
        if (typeof(T[]) == array.GetType()) {
          if (0 == length)
          return System.Array.Empty<T>();

          subarray = new T[length];
        } else subarray = System.Runtime.CompilerServices.Unsafe.As<T[]>(System.Array.CreateInstance(array.GetType(), length));

        System.Array.Copy(array, offset, subarray, 0, length);
        return subarray;
      }
    }
  }
#endif

/* ===================================== */
public static class Program {
  [UnityEngine.RuntimeInitializeOnLoadMethod]
  private static void Main() => new UnityEngine.GameObject("…", typeof(ProgramBehaviour));
}

internal sealed class ProgramBehaviour : UnityEngine.MonoBehaviour {
  public struct Alias<T> {
    public T Value;
  }

  #pragma warning disable CS0208
  #pragma warning disable CS8334

  public unsafe static class Aliaser<T> {
    public unsafe delegate void* RefAddresser(in T value);

    public static RefAddresser AddressOf = ((System.Func<RefAddresser>) (static () => { unsafe {
      System.Reflection.Emit.DynamicMethod method    = new("AddressOf", typeof(void*), new[] {typeof(T).MakeByRefType()}, typeof(Program).Module, true);
      System.Reflection.Emit.ILGenerator   generator = method.GetILGenerator();
      System.Reflection.Emit.LocalBuilder pinnedLocal = generator.DeclareLocal(typeof(void*), pinned: true);

      // … ⟶ Pray the Intermediate Language code is valid for C#’s .NET runtime and furthermore Unity’s Mono (IL2CPP) extension 🙏
      generator.Emit(System.Reflection.Emit.OpCodes.Ldarg_0);
        generator.Emit(System.Reflection.Emit.OpCodes.Stloc, pinnedLocal);
        generator.Emit(System.Reflection.Emit.OpCodes.Ldloc, pinnedLocal);
      generator.Emit(System.Reflection.Emit.OpCodes.Conv_U); // ⟶ Is the `unsigned` conversion necessary?
      generator.Emit(System.Reflection.Emit.OpCodes.Ret);

      return (RefAddresser) method.CreateDelegate(typeof(RefAddresser));
    } }))();

    public delegate object RefFieldDelegate(in Alias<T> alias);
    public static RefFieldDelegate GetAlias() {
      System.Reflection.Emit.DynamicMethod dm = new System.Reflection.Emit.DynamicMethod($"_Get_{typeof(Alias<T>).Name}_Value", typeof(object), new[] {typeof(Alias<T>).MakeByRefType()}, typeof(Alias<T>).Module, true);
      System.Reflection.Emit.ILGenerator il = dm.GetILGenerator();

      // …
      dm.DefineParameter(1, System.Reflection.ParameterAttributes.In, "alias");

      il.Emit(System.Reflection.Emit.OpCodes.Ldarg_0);
        // il.Emit(System.Reflection.Emit.OpCodes.Ldflda, typeof(Alias<T>).GetField(nameof(Alias<T>.Value), System.Reflection.BindingFlags.Instance | System.Reflection.BindingFlags.Public));
        il.Emit(System.Reflection.Emit.OpCodes.Ldfld, typeof(Alias<T>).GetField(nameof(Alias<T>.Value), System.Reflection.BindingFlags.Instance | System.Reflection.BindingFlags.Public));
        il.Emit(System.Reflection.Emit.OpCodes.Box, typeof(T));
      il.Emit(System.Reflection.Emit.OpCodes.Ret);

      return (RefFieldDelegate) dm.CreateDelegate(typeof(RefFieldDelegate));
    }

    public static T[] bruh = new T[] {default!};
  }

  public unsafe static ref T aliasof<T>(in Alias<T> alias) {
    // return ref System.Runtime.InteropServices.MemoryMarshal.AsRef<T>(System.Runtime.InteropServices.MemoryMarshal.CreateSpan(ref *(byte*) Aliaser<T>.GetAlias()(in alias), 1));
    // UnityEngine.Debug.Log($"[…]: {Aliaser<T>.GetAlias()(in alias)}");
    return ref Aliaser<T>.bruh[0];
    // return ref Aliaser<Alias<T>>.AddressOf(alias).Value;
  }

  private unsafe void Awake() {
    /* Do something… */
    System.IntPtr.Zero.ToPointer();
    UnityEngine.Debug.Log(aliasof(new Alias<object>() {Value = "Hello"}));
    // UnityEngine.Debug.Log(aliasof(new Alias<int>() {Value = 42}));
  }

  private void FixedUpdate() {}
  private void OnDestroy  () {}
}
