public static class Program {
  public struct A {
    public int n;
  }

  unsafe public static ref T const_cast<T>(in T value) where T : unmanaged {
    fixed (T* address = &value)
    return ref System.Runtime.InteropServices.MemoryMarshal.GetReference(new System.ReadOnlySpan<T>(address, 1));
  }

  /* Main */
  static void Main(string[] arguments) {
    System.Console.WriteLine(const_cast(new A() {n = 42}).n);
  }
}
