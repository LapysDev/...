#nullable enable annotations

/* ... */
public class Program {
  public class Tuple<T1, T2> : System.Runtime.CompilerServices.ITuple {
    private readonly T1  item1;
    private readonly T2  item2;
    public           int Length => 2;

    /* ... */
    public Tuple(T1 item1, T2 item2) {
      this.item1 = item1;
      this.item2 = item2;
    }

    /* ... */
    public override bool Equals(object? other) {
      return other is Tuple<T1, T2> tuple ? System.Object.Equals(this.item1, tuple.item1) && System.Object.Equals(this.item2, tuple.item2) : false;
    }

    public override int GetHashCode() {
      unchecked {
        int hash = 17;
        hash = (hash * 31) + (this.item1?.GetHashCode() ?? 0);
        hash = (hash * 31) + (this.item2?.GetHashCode() ?? 0);
        return hash;
      }
    }

    public override string ToString() {
      return $"({this.item1}, {this.item2})";
    }

    public object? this[int index] {
      get {
        return index switch {
          0 => this.item1,
          1 => this.item2,
          _ => throw new System.IndexOutOfRangeException("Index must be 0 or 1")
        };
      }
    }
  }

  /* Main */
  static void Main(System.String[] arguments) {
    Tuple<int, int> tuple = new(1, 3);

    // ...
    System.Console.WriteLine($"[]: {tuple}");
  }
}
