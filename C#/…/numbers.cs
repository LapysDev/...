public class Program {
  /* Main */
  static void Main(string[] arguments) {
    float normal    = 33.0f;
    float subnormal = 1.0f;

    // ...
    while (subnormal > float.Epsilon)
    subnormal /= 2.0f;

    System.Console.WriteLine($"[A]: {float.NaN}");
    System.Console.WriteLine($"[B]: {float.PositiveInfinity}");
    System.Console.WriteLine($"[C]: {float.NegativeInfinity}");
    System.Console.WriteLine($"[D]: {+0.0f}");
    System.Console.WriteLine($"[E]: {-0.0f}"); // --> float.NegativeZero
    System.Console.WriteLine($"[F]: {+subnormal}");
    System.Console.WriteLine($"[G]: {-subnormal}");
    System.Console.WriteLine($"[H]: {+normal}");
    System.Console.WriteLine($"[I]: {-normal}");
  }
}
