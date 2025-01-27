public class Program {
  /* Main */
  static void Main(string[] arguments) {
    System.Random randomizer = new();
    float[]       numbers    = System.Array.ConvertAll(new float[randomizer.Next(3, 7 + 1)], _ => (float) randomizer.NextDouble());
    byte[]        data       = new byte[numbers.Length * sizeof(float)];
    string        _          = "";

    // ...
    _ += "[numbers]: {"; foreach (float number in numbers) { _ += number.ToString("N6") + ", "; } _ += "}\r\n";

    System.Buffer.BlockCopy(numbers, 0, data, 0, numbers.Length * sizeof(float));
    _ += "[data]   : {"; foreach (byte datum in data) { _ += "0x" + datum.ToString("X") + ", "; } _ += "}\r\n";

    System.Buffer.BlockCopy(data, 0, numbers, 0, data.Length * sizeof(byte));
    _ += "[numbers]: {"; foreach (float number in numbers) { _ += number.ToString("N6") + ", "; } _ += "}\r\n";

    System.Console.Write(_);
  }
}
