#nullable enable annotations

/* ... */
public class Program {
  public class Dictionary<TKey, TValue> : System.Collections.Generic.IDictionary<TKey, TValue> {
    public struct Enumerator : System.Collections.Generic.IEnumerator<System.Collections.Generic.KeyValuePair<TKey, TValue>> {
      public           System.Collections.Generic.KeyValuePair<TKey, TValue> Current => this.current;
      private          System.Collections.Generic.KeyValuePair<TKey, TValue> current;
      private readonly Dictionary                             <TKey, TValue> dictionary;
      private          int                                                   index;
      System.Object                                                          System.Collections.IEnumerator.Current => Current;
      private          int                                                   version;

      /* … */
      public void Dispose() {}

      internal Enumerator(Dictionary<TKey, TValue> dictionary) {
        this.current    = default(System.Collections.Generic.KeyValuePair<TKey, TValue>);
        this.dictionary = dictionary;
        this.index      = 0;
        this.version    = dictionary.version;
      }

      public bool MoveNext() {
        if (this.dictionary.version != this.version)
        throw new System.InvalidOperationException($"Dictionary version {this.version} must be the same as Enumerator version {this.dictionary.version}");

        while (this.dictionary.count > this.index) {
          if (this.dictionary.hashes[this.index] >= 0) {
            this.current = new(this.dictionary.keys[index], this.dictionary.values[index]);
            this.index  += 1;

            return true;
          }

          ++this.index;
        }

        this.current = default(System.Collections.Generic.KeyValuePair<TKey, TValue>);
        this.index   = this.dictionary.count + 1;

        return false;
      }

      void System.Collections.IEnumerator.Reset() {
        if (this.dictionary.version != this.version)
        throw new System.InvalidOperationException($"Dictionary version {this.version} must be the same as Enumerator version {this.dictionary.version}");

        this.current = default(System.Collections.Generic.KeyValuePair<TKey, TValue>);
        this.index   = 0;
      }
    }

    /* … */
    public  System.Collections.Generic.Dictionary<TKey, TValue>         AsDictionary => new System.Collections.Generic.Dictionary<TKey, TValue>(this);
    private int[]                                                       buckets      =  null;
    private readonly System.Collections.Generic.IEqualityComparer<TKey> comparer     =  System.Collections.Generic.EqualityComparer<TKey>.Default;
    public  int                                                         Count        => this.count - this.freeCount;
    private int                                                         count        =  0;
    private int                                                         freeCount    =  0;
    private int                                                         freeList     =  0;
    private int[]                                                       hashes       =  null;
    public  bool                                                        IsReadOnly   => false;
    public  System.Collections.Generic.ICollection<TKey>                Keys         { get { TKey[] keys = new TKey[this.Count]; System.Array.Copy(this.keys, 0, keys, 0, this.Count); return keys; } }
    private TKey[]                                                      keys         = null;
    private int []                                                      next         = null;
    public  System.Collections.Generic.ICollection<TValue>              Values       { get { TValue[] values = new TValue[this.Count]; System.Array.Copy(this.values, 0, values, 0, this.Count); return values; } }
    private TValue[]                                                    values       = null;
    private int                                                         version      = 0;

    /* … */
    public Dictionary()                                                                : this(0,          null)     {}
    public Dictionary(int                                                  capacity)   : this(capacity,   null)     {}
    public Dictionary(System.Collections.Generic.IEqualityComparer<TKey>   comparer)   : this(0,          comparer) {}
    public Dictionary(System.Collections.Generic.IDictionary<TKey, TValue> dictionary) : this(dictionary, null)     {}

    public Dictionary(int capacity, System.Collections.Generic.IEqualityComparer<TKey> comparer) {
      if (capacity < 0) throw new System.ArgumentOutOfRangeException("Dictionary capacity is less than 0");

      this.Initialize(capacity);
      this.comparer = comparer ?? System.Collections.Generic.EqualityComparer<TKey>.Default;
    }

    public Dictionary(System.Collections.Generic.IDictionary<TKey, TValue> dictionary, System.Collections.Generic.IEqualityComparer<TKey> comparer) : this(null != dictionary ? dictionary.Count : 0, comparer) {
      if (null == dictionary) throw new System.ArgumentNullException("Dictionary dictionary is null");

      foreach (System.Collections.Generic.KeyValuePair<TKey, TValue> current in dictionary)
      this.Add(current.Key, current.Value);
    }

    /* … */
    public void Add(System.Collections.Generic.KeyValuePair<TKey, TValue> item) {
      this.Add(item.Key, item.Value);
    }

    public void Add(TKey key, TValue value) {
      this.Insert(key, value, true);
    }

    public void Clear() {
      if (this.count <= 0)
      return;

      for (int index = 0; index != this.buckets.Length; ++index)
        this.buckets[index] = -1;

      System.Array.Clear(this.hashes, 0, this.count);
      System.Array.Clear(this.keys,   0, this.count);
      System.Array.Clear(this.next,   0, this.count);
      System.Array.Clear(this.values, 0, this.count);

      this.count     = 0;
      this.freeCount = 0;
      this.freeList  = -1;
      this.version  += 1;
    }

    public bool Contains(System.Collections.Generic.KeyValuePair<TKey, TValue> item) {
      int index = this.FindIndex(item.Key);
      return index >= 0 && System.Collections.Generic.EqualityComparer<TValue>.Default.Equals(this.values[index], item.Value);
    }

    public bool ContainsKey(TKey key) {
      return this.FindIndex(key) >= 0;
    }

    public bool ContainsValue(TValue value) {
      System.Func<TValue?, TValue?, bool> comparer = null == value ? (x, y) => null == x : System.Collections.Generic.EqualityComparer<TValue>.Default.Equals;

      // …
      for (int index = 0; index != this.count; ++index) {
        if (this.hashes[index] >= 0 && comparer(this.values[index], value))
        return true;
      }

      return false;
    }

    public void CopyTo(System.Collections.Generic.KeyValuePair<TKey, TValue>[] array, int offset) {
      if (null == array)                     throw new System.ArgumentNullException        ($"Array is null");
      if (offset < 0 || offset > array.Length) throw new System.ArgumentOutOfRangeException($"Dictionary.CopyTo(…) {{index: {offset}, array: {{Length: {array.Length}}}}}");
      if (Count > array.Length - offset)      throw new System.ArgumentException           ($"The number of elements in the dictionary ({Count}) is greater than the available space from offset to the end of the destination array ({array.Length})");

      for (int index = 0; index != this.count; ++index) {
        if (this.hashes[index] >= 0)
        array[offset++] = new(this.keys[index], this.values[index]);
      }
    }

    private int FindIndex(TKey key) {
      if (null == key)
      throw new System.ArgumentNullException("Key is null");

      if (null != this.buckets) {
        int hash = this.comparer.GetHashCode(key) & 0x7FFFFFFF;

        for (int index = this.buckets[hash % this.buckets.Length]; index >= 0; index = this.next[index]) {
          if (hash == this.hashes[index] && this.comparer.Equals(this.keys[index], key))
          return index;
        }
      }

      return -1;
    }

    public Enumerator GetEnumerator() {
      return new(this);
    }

    public static int GetPrime(int minimum) {
      if (minimum < 0)
      throw new System.ArgumentException("Prime minimum is less than 0");

      foreach (int prime in new[] {3, 7, 11, 17, 23, 29, 37, 47, 59, 71, 89, 107, 131, 163, 197, 239, 293, 353, 431, 521, 631, 761, 919, 1103, 1327, 1597, 1931, 2333, 2801, 3371, 4049, 4861, 5839, 7013, 8419, 10103, 12143, 14591, 17519, 21023, 25229, 30293, 36353, 43627, 52361, 62851, 75431, 90523, 108631, 130363, 156437, 187751, 225307, 270371, 324449, 389357, 467237, 560689, 672827, 807403, 968897, 1162687, 1395263, 1674319, 2009191, 2411033, 2893249, 3471899, 4166287, 4999559, 5999471, 7199369}) {
        if (minimum <= prime)
        return prime;
      }

      for (int index = minimum | 1; index != 0x7FFFFFFF; index += 2) {
        if (index == 2)
        return index;

        if (0 != (index & 1)) {
          int  limit  = (int) System.Math.Sqrt((double) index);
          bool primed = true;

          // …
          for (int subindex = 3; limit >= subindex; subindex += 2)
          if (0 == index % subindex) {
            primed = false;
            break;
          }

          if (primed && 0 != (index - 1) % 101)
          return index;
        }
      }

      return minimum;
    }

    private void Initialize(int capacity) {
      int prime = Dictionary<TKey, TValue>.GetPrime(capacity);

      // …
      this.buckets  = new int[prime];
      this.freeList = -1;
      this.hashes   = new int   [prime];
      this.keys     = new TKey  [prime];
      this.next     = new int   [prime];
      this.values   = new TValue[prime];

      for (int index = 0; index != this.buckets.Length; ++index)
      this.buckets[index] = -1;
    }

    private void Insert(TKey key, TValue value, bool insert) {
      int hash      = 0;
      int hashIndex = 0;
      int number    = 0;

      // …
      if (null == key)
        throw new System.ArgumentNullException("Key is null");

      if (null == this.buckets)
        this.Initialize(0);

      hash      = this.comparer.GetHashCode(key) & 0x7FFFFFFF;
      hashIndex = hash % this.buckets.Length;

      for (int index = this.buckets[hashIndex]; index >= 0; index = next[index]) {
        if (hash == this.hashes[index] && this.comparer.Equals(this.keys[index], key)) {
          if (insert) throw new System.ArgumentException($"Key already exists: `{key}`");

          this.values[index] = value;
          this.version      += 1;
          return;
        }

        ++number;
      }

      if (this.freeCount > 0) {
        number          = this.freeList;
        this.freeCount -= 1;
        this.freeList   = this.next[number];
      }

      else {
        if (this.count == this.keys.Length) {
          this.Resize();
          hashIndex = hash % this.buckets.Length;
        }

        number      = this.count;
        this.count += 1;
      }

      this.next   [number]    = this.buckets[hashIndex];
      this.buckets[hashIndex] = number;
      this.hashes [number]    = hash;
      this.keys   [number]    = key;
      this.values [number]    = value;
      this.version           += 1;
    }

    public bool Remove(TKey key) {
      int hash      = 0;
      int hashIndex = 0;
      int number    = -1;

      // …
      if (key == null)
        throw new System.ArgumentNullException("Key is null");

      hash      = this.comparer.GetHashCode(key) & 0x7FFFFFFF;
      hashIndex = hash % this.buckets.Length;

      for (int index = this.buckets[hashIndex]; index >= 0; index = this.next[index]) {
        if (hash == this.hashes[index] && this.comparer.Equals(this.keys[index], key)) {
          if (number < 0) this.buckets[hashIndex] = this.next[index];
          else            this.next   [number]    = this.next[index];

          this.version      += 1;
          this.values[index] = default(TValue);
          this.next  [index] = this.freeList;
          this.keys  [index] = default(TKey);
          this.hashes[index] = -1;
          this.freeList      = index;
          this.freeCount    += 1;

          return true;
        }

        number = index;
      }

      return false;
    }

    public bool Remove(System.Collections.Generic.KeyValuePair<TKey, TValue> item) {
      return this.Remove(item.Key);
    }

    private void Resize() {
      this.Resize(this.count < 0x7FEFFFFD && this.count * 2 > 0x7FEFFFFD ? 0x7FEFFFFD : Dictionary<TKey, TValue>.GetPrime(this.count * 2), false);
    }

    private void Resize(int capacity, bool regenerateHashes) {
      int[]    buckets = new int   [capacity];
      int[]    hashes  = new int   [capacity];
      TKey[]   keys    = new TKey  [capacity];
      int[]    next    = new int   [capacity];
      TValue[] values  = new TValue[capacity];

      // …
      for (int index = 0; buckets.Length != index; ++index)
        buckets[index] = -1;

      System.Array.Copy(this.hashes, 0, hashes, 0, this.count);
      System.Array.Copy(this.keys,   0, keys,   0, this.count);
      System.Array.Copy(this.next,   0, next,   0, this.count);
      System.Array.Copy(this.values, 0, values, 0, this.count);

      if (regenerateHashes)
      for (int index = 0; index != this.count; index++) {
        if (hashes[index] != -1)
        hashes[index] = this.comparer.GetHashCode(keys[index]) & 0x7FFFFFFF;
      }

      for (int index = 0; index != this.count; index++) {
        int hashIndex = hashes[index] % capacity;

        // …
        next   [index]     = buckets[hashIndex];
        buckets[hashIndex] = index;
      }

      this.buckets = buckets;
      this.hashes  = hashes;
      this.keys    = keys;
      this.next    = next;
      this.values  = values;
    }

    System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator() {
      return this.GetEnumerator();
    }

    System.Collections.Generic.IEnumerator<System.Collections.Generic.KeyValuePair<TKey,TValue>> System.Collections.Generic.IEnumerable<System.Collections.Generic.KeyValuePair<TKey,TValue>>.GetEnumerator() {
      return this.GetEnumerator();
    }

    public bool TryGetValue(TKey key, out TValue value) {
      int index = this.FindIndex(key);

      // …
      if (index >= 0) {
        value = this.values[index];
        return true;
      }

      value = default(TValue);
      return false;
    }

    public TValue this[TKey key] {
      get {
        int index = this.FindIndex(key);

        if (index >= 0) return this.values[index];
        throw new System.Collections.Generic.KeyNotFoundException(key.ToString());
      }

      set {
        this.Insert(key, value, false);
      }
    }

    public TValue this[TKey key, TValue _] {
      get {
        int index = this.FindIndex(key);
        return index >= 0 ? this.values[index] : _;
      }
    }
  }

  /* Main */
  static void Main(System.String[] arguments) {
    Dictionary<string, int> dictionary = new() {{"A", 42}, {"B", 1337}};

    // ...
    static void Log(Dictionary<string, int> dictionary) {
      System.Collections.Generic.ICollection<string> keys   = dictionary.Keys;
      System.Collections.Generic.ICollection<int>    values = dictionary.Values;

      // ...
      System.Console.WriteLine($"({dictionary.Count}) {{Keys: ({keys.Count}), Values: ({dictionary.Values.Count})}}");

      foreach (System.Collections.Generic.KeyValuePair<string, int> item in dictionary)
        System.Console.WriteLine("   " + $"{item.Key} => {item.Value}");

      System.Console.Write("  Keys  : ["); foreach (string key   in keys)   System.Console.Write($"{key}"   + ", "); System.Console.WriteLine("..]");
      System.Console.Write("  Values: ["); foreach (int    value in values) System.Console.Write($"{value}" + ", "); System.Console.WriteLine("..]");
    }

    /* ... */
    Log(dictionary);
    System.Console.WriteLine($"[A (val.)]: {dictionary["A"]}");
    System.Console.WriteLine();

    dictionary.Add("C", 3);
    Log(dictionary);
    System.Console.WriteLine($"[C (val.)]: {dictionary["C"]}");
    System.Console.WriteLine();

    System.Console.WriteLine($"[B    (has)]: {dictionary.ContainsKey  ("B")}");
    System.Console.WriteLine($"[D    (has)]: {dictionary.ContainsKey  ("D")}");
    System.Console.WriteLine($"[1337 (has)]: {dictionary.ContainsValue(1337)}");
    System.Console.WriteLine($"[69   (has)]: {dictionary.ContainsValue(69)}");
    System.Console.WriteLine();

    dictionary.Clear();
    Log(dictionary);
    System.Console.WriteLine();

    dictionary.Add("D", 69);
    Log(dictionary);
    System.Console.WriteLine($"[D  (val.)]: {dictionary              ["D"]}");
    System.Console.WriteLine($"[D  (has)] : {dictionary.ContainsKey  ("D")}");
    System.Console.WriteLine($"[69 (has)] : {dictionary.ContainsValue(69)}");
    System.Console.WriteLine();

    System.Console.WriteLine($"[D (del.)]: {dictionary.Remove("D")}");
    Log(dictionary);
    System.Console.WriteLine();

    System.Console.WriteLine($"[D (del.)]: {dictionary.Remove("D")}");
    Log(dictionary);
  }
}
