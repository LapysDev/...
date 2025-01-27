#nullable enable annotations

public class Program {
  /* Main */
  static void Main(string[] arguments) {
    System.Threading.Timer       timer     = null;
    System.Diagnostics.Stopwatch stopwatch = System.Diagnostics.Stopwatch.StartNew();

    // ...
    new System.Threading.Timer(_ => { System.Console.WriteLine("Timer (interval) concluded"); }, null, 0, 1500);
    timer = new System.Threading.Timer(_ => { timer.Dispose(); System.Console.WriteLine("Timer (timeout) concluded"); }, null, 3000, System.Threading.Timeout.Infinite);

    System.Threading.Thread.Sleep(3000 + /* ->> Timeout leeway */ 100);
    System.Console.WriteLine("Sleep (" + stopwatch.ElapsedTicks + " ticks) concluded");
    stopwatch.Stop();
  }
}
