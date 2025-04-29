public static class Program {
  [UnityEngine.RuntimeInitializeOnLoadMethod]
  private static void Main() => new UnityEngine.GameObject("…", typeof(ProgramBehaviour));
}

internal sealed class ProgramBehaviour : UnityEngine.MonoBehaviour {
  private struct Ref<T> {
    public delegate ref T Delegate(T[] array, int offset);
    public static Ref<T>.Delegate OffsetOf = (Ref<T>.Delegate) (
      typeof(T).IsValueType // and other edge-cases
      ? ((Ref<int>.Delegate) Ref<int>.UnmanagedOffsetOf).Method.GetGenericMethodDefinition().MakeGenericMethod(typeof(T)).CreateDelegate(typeof(Ref<T>.Delegate))
      : (Ref<T>.Delegate) Ref<T>.ManagedOffsetOf<T>
    );

    /* ... */
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)]
    public static ref U ManagedOffsetOf<U>(U[] array, int offset) {
      return ref array[offset];
    }

    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)]
    public unsafe static ref U UnmanagedOffsetOf<U>(U[] array, int offset) where U : unmanaged {
      fixed (U* address = &System.Runtime.InteropServices.MemoryMarshal.GetReference(new System.ReadOnlySpan<U>(array)))
      return ref *(address + offset);
    }
  }

  /* ... */
  class A { public int n; }

  private unsafe void Awake() {
    A[]   array = new[] {new A{n = 1}, new A{n = 2}, new A{n = 3}, new A{n = 4}, new A{n = 5}}; // ->> Could also be `stackalloc`’d
    ref A a1    = ref System.Runtime.InteropServices.MemoryMarshal.GetReference(new System.ReadOnlySpan<A>(array));
    ref A a2    = ref Unity.Collections.LowLevel.Unsafe.UnsafeUtility.ArrayElementAsRef<A>(Unity.Collections.LowLevel.Unsafe.UnsafeUtility.PinGCArrayAndGetDataAddress(array, out ulong handle), 1); // ->> Internally uses `System.Runtime.InteropServices(…, System.Runtime.InteropServices.GCHandleType.Pinned)`
    ref A a3    = ref Ref<A>.OffsetOf(array, 2);
    ref A a4    = ref System.Runtime.InteropServices.MemoryMarshal.CreateSpan(ref a1, 4)[3]; // ->> Slower than `ref array[2]`, prefer `ref System.Runtime.CompilerServices.Unsafe.Add(ref a1, 2)`
    ref A a5    = ref array[4]; // ->> Slower than `ref array[2]`, prefer `ref System.Runtime.CompilerServices.Unsafe.Add(ref a1, 2)`

    // ...
    a1.n = 6;  UnityEngine.Debug.Log(array[0].n);
    a2.n = 7;  UnityEngine.Debug.Log(array[1].n);
    a3.n = 8;  UnityEngine.Debug.Log(array[2].n);
    a4.n = 9;  UnityEngine.Debug.Log(array[3].n);
    a5.n = 10; UnityEngine.Debug.Log(array[4].n);

    // Unity.Collections.LowLevel.Unsafe.UnsafeUtility.ReleaseGCObject(handle);
  }
}
