public class Program {
  class Items {
    public Enumerator GetEnumerator() => new();

    public class Enumerator {
      private int[] items;
      private int   position;
      public  int   Current => this.items[position];

      public Enumerator() {
        this.items    = new[] {1, 2, 3, 4, 5};
        this.position = -1;
      }

      public bool MoveNext() => ++this.position < items.Length;
    }
  }

  /* Main */
  static void Main(System.String[] arguments) {
    foreach (int item in new Items())
    System.Console.WriteLine($"[..]: {item}");

    for (var enumerator = new Items().GetEnumerator(); enumerator.MoveNext(); )
    System.Console.WriteLine($"[...]: {enumerator.Current}");
  }
}
