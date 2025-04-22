public static class Program {
  public class A     { /* ->> Empty */ }
  public class B     { public     short   n; }
  public class C : B { public new decimal n; }

  /* Main */
  static void Main(string[] arguments) {
    // ... --> sizeof(void*)
    #pragma warning disable CS8500
    unsafe { System.Console.WriteLine($"{sizeof(A)} {sizeof(B)} {sizeof(C)}"); }
    #pragma warning restore CS8500

    // ... --> sizeof(void*) ->> Requires unmanaged type
    try { System.Console.WriteLine($"{System.Runtime.InteropServices.Marshal.SizeOf<A>()} {System.Runtime.InteropServices.Marshal.SizeOf<B>()} {System.Runtime.InteropServices.Marshal.SizeOf<C>()}"); }
    catch (System.ArgumentException) { System.Console.WriteLine("..."); }

    // ... --> sizeof(void*)
    System.Console.WriteLine($"{System.Runtime.CompilerServices.Unsafe.SizeOf<A>()} {System.Runtime.CompilerServices.Unsafe.SizeOf<B>()} {System.Runtime.CompilerServices.Unsafe.SizeOf<C>()}");

    // ... ->> Imperfect
    long size = 0L; {
      size = System.GC.GetTotalMemory(true); new A(); System.Console.Write($"{System.GC.GetTotalMemory(true) - size} ");
      size = System.GC.GetTotalMemory(true); new B(); System.Console.Write($"{System.GC.GetTotalMemory(true) - size} ");
      size = System.GC.GetTotalMemory(true); new C(); System.Console.Write($"{System.GC.GetTotalMemory(true) - size} ");
    } System.Console.WriteLine();
  }
}
