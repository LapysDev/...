public static class Program {
  public struct T {
    public int n;
  }

  /* Main */
  static void Main(string[] arguments) {
    System.Span<T> array     = stackalloc T[] {new() {n = 1}, new() {n = 2}, new() {n = 3}}; // ->> Either works
    T[]            array     = new        T[] {new() {n = 1}, new() {n = 2}, new() {n = 3}}; // ->> Either works
    ref T          reference = ref System.Runtime.InteropServices.MemoryMarshal.GetReference(array);

    // ...
    for (int index = 0; index != array.Length; ++index) {
      ref T element = ref System.Runtime.CompilerServices.Unsafe.Add(ref reference, index);
      element.n *= 2;
    }

    foreach (T _ in array)
    System.Console.Write($"{_.n} ");
  }
}
