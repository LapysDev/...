#nullable enable annotations

namespace PatchOdyssey {
  #if true
    [System.Serializable]
    public class SerializedDictionary<TKey, TValue> : System.Collections.Generic.IDictionary<TKey, TValue> /* → `https://discussions.unity.com/t/finally-a-serializable-dictionary-for-unity-extracted-from-system-collections-generic/586385/1` */ {
      public struct Enumerator : System.Collections.Generic.IEnumerator<System.Collections.Generic.KeyValuePair<TKey, TValue>> {
        public           System.Collections.Generic.KeyValuePair<TKey, TValue> Current => this.current;
        private          System.Collections.Generic.KeyValuePair<TKey, TValue> current;
        private readonly PatchOdyssey.SerializedDictionary      <TKey, TValue> dictionary;
        private          int                                                   index;
        public           int                                                   version;
        object                                                                 System.Collections.IEnumerator.Current => Current;

        /* … */
        internal Enumerator(PatchOdyssey.SerializedDictionary<TKey, TValue> dictionary) {
          this.current    = default;
          this.dictionary = dictionary;
          this.index      = 0;
          this.version    = dictionary.version;
        }

        /* … */
        public void Dispose() {}

        public bool MoveNext() {
          if (this.dictionary.version != this.version)
          throw new System.InvalidOperationException($"Dictionary version {this.version} must be the same as Enumerator version {this.dictionary.version}");

          for (; this.dictionary.count > this.index; ++this.index)
          if (this.dictionary.hashes[this.index] >= 0) {
            this.current = new(this.dictionary.keys[index], this.dictionary.values[index]);
            this.index  += 1;

            return true;
          }

          this.current = default;
          this.index   = this.dictionary.count + 1;

          return false;
        }

        void System.Collections.IEnumerator.Reset() {
          if (this.dictionary.version != this.version)
          throw new System.InvalidOperationException($"Dictionary version {this.version} must be the same as Enumerator version {this.dictionary.version}");

          this.current = default;
          this.index   = 0;
        }
      }

      /* … */
      /*[UnityEngine.HideInInspector, UnityEngine.SerializeField]*/ private int[]                                               buckets      =  null;
      private readonly                                                  System.Collections.Generic.IEqualityComparer<TKey>  comparer     =  System.Collections.Generic.EqualityComparer<TKey>.Default;
      public                                                            int                                                 Count        => this.count - this.freeCount;
      /*[UnityEngine.HideInInspector, UnityEngine.SerializeField]*/ private int                                                 count        =  0;
      /*[UnityEngine.HideInInspector, UnityEngine.SerializeField]*/ private int                                                 freeCount    =  0;
      /*[UnityEngine.HideInInspector, UnityEngine.SerializeField]*/ private int                                                 freeList     =  0;
      /*[UnityEngine.HideInInspector, UnityEngine.SerializeField]*/ private int[]                                               hashes       =  null;
      public                                                            System.Collections.Generic.ICollection<TKey>        Keys         { get { TKey[] keys = new TKey[this.Count]; System.Array.Copy(this.keys, 0, keys, 0, this.Count); return keys; } }
      /*[UnityEngine.HideInInspector, UnityEngine.SerializeField]*/ private TKey[]                                              keys         = null;
      /*[UnityEngine.HideInInspector, UnityEngine.SerializeField]*/ private int []                                              next         = null;
      public                                                            System.Collections.Generic.ICollection<TValue>      Values       { get { TValue[] values = new TValue[this.Count]; System.Array.Copy(this.values, 0, values, 0, this.Count); return values; } }
      /*[UnityEngine.HideInInspector, UnityEngine.SerializeField]*/ private TValue[]                                            values       = null;
      /*[UnityEngine.HideInInspector, UnityEngine.SerializeField]*/ public  int                                                 version      = 0;
      bool                                                                                                                  System.Collections.Generic.ICollection<System.Collections.Generic.KeyValuePair<TKey,TValue>>.IsReadOnly => false;

      /* … */
      public SerializedDictionary()                                                                : this(0,          null)     {}
      public SerializedDictionary(int                                                  capacity)   : this(capacity,   null)     {}
      public SerializedDictionary(System.Collections.Generic.IEqualityComparer<TKey>   comparer)   : this(0,          comparer) {}
      public SerializedDictionary(System.Collections.Generic.IDictionary<TKey, TValue> dictionary) : this(dictionary, null)     {}

      public SerializedDictionary(int capacity, System.Collections.Generic.IEqualityComparer<TKey> comparer) {
        if (capacity < 0)
          throw new System.ArgumentOutOfRangeException($"SerializedDictionary capacity is less than 0");

        this.Initialize(capacity);
        this.comparer = comparer ?? System.Collections.Generic.EqualityComparer<TKey>.Default;
      }

      public SerializedDictionary(System.Collections.Generic.IDictionary<TKey, TValue> dictionary, System.Collections.Generic.IEqualityComparer<TKey> comparer) : this(null != dictionary ? dictionary.Count : 0, comparer) {
        if (null == dictionary) throw new System.ArgumentNullException($"SerializedDictionary dictionary is null");
        foreach (System.Collections.Generic.KeyValuePair<TKey, TValue> current in dictionary) this.Add(current.Key, current.Value);
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
        if (null == array)                       throw new System.ArgumentNullException      ($"Array is null");
        if (offset < 0 || offset > array.Length) throw new System.ArgumentOutOfRangeException($"SerializedDictionary.CopyTo(…) {{index: {offset}, array: {{Length: {array.Length}}}}}");
        if (Count > array.Length - offset)       throw new System.ArgumentException          ($"The number of elements in the dictionary ({Count}) is greater than the available space from offset to the end of the destination array ({array.Length})");

        for (int index = 0; index != this.count; ++index) {
          if (this.hashes[index] >= 0)
          array[offset++] = new(this.keys[index], this.values[index]);
        }
      }

      private int FindIndex(TKey key) {
        if (null == key)
        throw new System.ArgumentNullException($"Dictionary key is null");

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
        throw new System.ArgumentException($"Dictionary prime minimum is less than 0");

        foreach (int prime in new[] {3, 7, 11, 17, 23, 29, 37, 47, 59, 71, 89, 107, 131, 163, 197, 239, 293, 353, 431, 521, 631, 761, 919, 1103, 1327, 1597, 1931, 2333, 2801, 3371, 4049, 4861, 5839, 7013, 8419, 10103, 12143, 14591, 17519, 21023, 25229, 30293, 36353, 43627, 52361, 62851, 75431, 90523, 108631, 130363, 156437, 187751, 225307, 270371, 324449, 389357, 467237, 560689, 672827, 807403, 968897, 1162687, 1395263, 1674319, 2009191, 2411033, 2893249, 3471899, 4166287, 4999559, 5999471, 7199369}) {
          if (minimum <= prime)
          return prime;
        }

        for (int index = minimum | 1; index < 0x7FFFFFFF; index += 2) {
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
        int prime = SerializedDictionary<TKey, TValue>.GetPrime(capacity);

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
          throw new System.ArgumentNullException($"Dictionary key is `null`");

        if (null == this.buckets)
          this.Initialize(0);

        hash      = this.comparer.GetHashCode(key) & 0x7FFFFFFF;
        hashIndex = hash % this.buckets.Length;

        for (int index = this.buckets[hashIndex]; index >= 0; index = next[index]) {
          if (hash == this.hashes[index] && this.comparer.Equals(this.keys[index], key)) {
            if (insert) throw new System.ArgumentException($"Dictionary key already exists: `{key}`");

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
          throw new System.ArgumentNullException($"Dictionary key is null");

        hash      = this.comparer.GetHashCode(key) & 0x7FFFFFFF;
        hashIndex = hash % this.buckets.Length;

        for (int index = this.buckets[hashIndex]; index >= 0; index = this.next[index], number = index)
        if (hash == this.hashes[index] && this.comparer.Equals(this.keys[index], key)) {
          if (number < 0) this.buckets[hashIndex] = this.next[index];
          else            this.next   [number]    = this.next[index];

          this.version      += 1;
          this.values[index] = default;
          this.next  [index] = this.freeList;
          this.keys  [index] = default;
          this.hashes[index] = -1;
          this.freeList      = index;
          this.freeCount    += 1;

          return true;
        }

        return false;
      }

      public bool Remove(System.Collections.Generic.KeyValuePair<TKey, TValue> item) {
        return this.Remove(item.Key);
      }

      private void Resize() {
        this.Resize(this.count < 0x7FEFFFFD && this.count * 2 > 0x7FEFFFFD ? 0x7FEFFFFD : SerializedDictionary<TKey, TValue>.GetPrime(this.count * 2), false);
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

        value = default;
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
  #else
    /* TODO (Lapys) -> Make work. Fuck you Microsoft */
    // → `https://github.com/dotnet/runtime/blob/main/src/libraries/System.Private.CoreLib/src/System/Collections/Generic/Dictionary.cs`
    [System.Diagnostics.DebuggerDisplay("Count = {Count}")]
    [System.Serializable]
    public class Dictionary<TKey, TValue> : System.Collections.Generic.IDictionary<TKey, TValue>, System.Collections.IDictionary, System.Collections.Generic.IReadOnlyDictionary<TKey, TValue>, System.Runtime.Serialization.ISerializable, System.Runtime.Serialization.IDeserializationCallback where TKey : notnull {
      // → Provides a type that may be used to perform operations on a `PatchOdyssey.Dictionary<TKey, TValue>` using an `TAlternateKey` as a key instead of a `TKey`
      public readonly struct AlternateLookup<TAlternateKey> where TAlternateKey : notnull /* allows ref struct */ {
        public /* required */ PatchOdyssey.Dictionary<TKey, TValue> Dictionary { get; } // → Gets the `PatchOdyssey.Dictionary<TKey, TValue>` against which this instance performs operations

        /* … */
        // → Initialize the instance. The dictionary must have already been verified to have a compatible `comparer`
        internal AlternateLookup(PatchOdyssey.Dictionary<TKey, TValue> dictionary) {
          this.Dictionary = dictionary;
        }

        #if NET9_0_OR_GREATER
          // → Gets the dictionary's alternate `comparer`; The dictionary must have already been verified as compatible
          // → Determines whether the `PatchOdyssey.Dictionary<TKey, TValue>` contains the specified alternate key
          public bool ContainsKey(TAlternateKey key) {
            return !System.Runtime.CompilerServices.Unsafe.IsNullRef(ref this.FindValue(key, out _));
          }

          // → Finds the entry associated with the specified alternate key
          internal ref TValue FindValue(TAlternateKey key, /* [System.Diagnostics.CodeAnalysis.MaybeNullWhen(false)] */ out TKey dictionaryKey) {
            System.Collections.Generic.IAlternateEqualityComparer<TAlternateKey, TKey> comparer = this.GetAlternateComparer(this.Dictionary);
            ref Entry                                                                  entry    = ref System.Runtime.CompilerServices.Unsafe.NullRef<Entry>();

            // …
            if (null != this.Dictionary.buckets) {
              uint     collisionCount = 0u;
              Entry[]? entries        = this.Dictionary.entries;
              uint     hashCode       = (uint) comparer.GetHashCode(key);
              int      index          = this.Dictionary.GetBucket(hashCode) - 1;

              do {
                if ((uint) entries.Length <= (uint) index)
                goto ReturnNotFound;

                entry = ref entries[index];

                if (entry.hashCode == hashCode && comparer.Equals(key, entry.key))
                goto ReturnFound;

                index = entry.next;
              } while (++collisionCount <= (uint)entries.Length);

              goto ConcurrentOperation;
            }

            goto ReturnNotFound;

            ConcurrentOperation:
              throw new System.InvalidOperationException("Lorem ipsum");

            ReturnFound:
              ref TValue value = ref entry.value;
              dictionaryKey = entry.key;

            Return:
              return ref value;

            ReturnNotFound:
              dictionaryKey = default!;
              value         = ref System.Runtime.CompilerServices.Unsafe.NullRef<TValue>();

            goto Return;
          }

          [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)]
          internal static System.Collections.Generic.IAlternateEqualityComparer<TAlternateKey, TKey> GetAlternateComparer(PatchOdyssey.Dictionary<TKey, TValue> dictionary) {
            return System.Runtime.CompilerServices.Unsafe.As<System.Collections.Generic.IAlternateEqualityComparer<TAlternateKey, TKey>>(dictionary.comparer);
          }

          internal ref TValue? GetValueRefOrAddDefault(TAlternateKey key, out bool exists) {
            if (null == this.Dictionary.buckets) this.Dictionary.Initialize(0);
            System.Collections.Generic.IAlternateEqualityComparer<TAlternateKey, TKey> comparer       = this.GetAlternateComparer(this.Dictionary);
            uint                                                                       hashCode       = (uint) comparer.GetHashCode(key);
            uint                                                                       collisionCount = 0;
            ref int                                                                    bucket         = ref this.Dictionary.GetBucket(hashCode);
            int                                                                        index          = bucket - 1; // → Value in `this.buckets` is one-based

            // …
            while ((uint) index < (uint) this.Dictionary.entries.Length) {
              if (hashCode == this.Dictionary.entries[index].hashCode && comparer.Equals(key, this.Dictionary.entries[index].key)) {
                exists = true;
                return ref this.Dictionary.entries[index].value!;
              }

              index = this.Dictionary.entries[index].next;

              if (++collisionCount > (uint)this.Dictionary.entries.Length)
              throw new System.InvalidOperationException("Lorem ipsum");
            }

            // …
            TKey dictionaryKey = comparer.Create(key);

            if (dictionaryKey is null)
            throw new System.ArgumentNullException("Lorem ipsum");

            if (this.Dictionary.freeCount > 0) {
              index                      = this.Dictionary.freeList;
              this.Dictionary.freeList   = Dictionary<TKey, TValue>.StartOfFreeList - this.Dictionary.entries[this.Dictionary.freeList].next;
              this.Dictionary.freeCount -= 1;
            }

            else {
              int count = this.Dictionary.count;

              // …
              if (count == this.Dictionary.entries.Length) {
                this.Dictionary.Resize();
                bucket = ref this.Dictionary.GetBucket(hashCode);
              }

              index                 = count;
              this.Dictionary.count = count + 1;
            }

            // …
            ref Entry entry = ref this.Dictionary.entries![index];

            entry.value              = default!;
            entry.next               = bucket - 1; // → Value in `this.buckets` is one-based
            entry.key                = dictionaryKey;
            entry.hashCode           = hashCode;
            bucket                   = index + 1; // → Value in `this.buckets` is one-based
            this.Dictionary.version += 1;

            if (!typeof(TKey).IsValueType && collisionCount > Hash.HashCollisionThreshold && comparer is NonRandomizedStringEqualityComparer) {
              // → Value types never rehash
              this.Dictionary.Resize(this.Dictionary.entries.Length, true);
              exists = false;

              // → At this point the `this.Dictionary.entries` array has been resized, so the current reference we have is no longer valid so we're forced to do a new lookup and return an updated reference to the new entry instance. This new lookup is guaranteed to always find a value though and it will never return a null reference here
              return ref this.Dictionary.FindValue(dictionaryKey)!;
            }

            exists = false;

            return ref entry.value!;
          }

          // → Checks whether the dictionary has a `comparer` compatible with `TAlternateKey`
          [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)]
          internal static bool IsCompatibleKey(PatchOdyssey.Dictionary<TKey, TValue> dictionary) {
            return dictionary.comparer is System.Collections.Generic.IAlternateEqualityComparer<TAlternateKey, TKey>;
          }

          // → Removes the value with the specified alternate key from the `PatchOdyssey.Dictionary<TKey, TValue>`
          public bool Remove(TAlternateKey key) {
            return this.Remove(key, out _, out _);
          }

          public bool Remove(TAlternateKey key, /* [System.Diagnostics.CodeAnalysis.MaybeNullWhen(false)] */ out TKey dictionaryKey, /* [System.Diagnostics.CodeAnalysis.MaybeNullWhen(false)] */ out TValue value) {
            System.Collections.Generic.IAlternateEqualityComparer<TAlternateKey, TKey> comparer = this.GetAlternateComparer(this.Dictionary);

            // …
            if (this.Dictionary.buckets != null) {
              uint     collisionCount = 0u;
              uint     hashCode       = (uint) comparer.GetHashCode(key);
              ref int  bucket         = ref this.Dictionary.GetBucket(hashCode);
              int      index          = bucket - 1; // → Value in `this.buckets` is one-based
              int      recentIndex    = -1;

              // …
              while (index >= 0) {
                ref Entry entry = ref this.Dictionary.entries[index];

                // …
                if (entry.hashCode == hashCode && comparer.Equals(key, entry.key)) {
                  if (recentIndex < 0) bucket = entry.next + 1; // → Value in `this.buckets` is one-based
                  else this.Dictionary.entries[recentIndex].next = entry.next;

                  dictionaryKey = entry.key;
                  value         = entry.value;
                  entry.next    = Dictionary<TKey, TValue>.StartOfFreeList - this.Dictionary.freeList;

                  if (System.Runtime.CompilerServices.RuntimeHelpers.IsReferenceOrContainsReferences<TKey>())   entry.key   = default!;
                  if (System.Runtime.CompilerServices.RuntimeHelpers.IsReferenceOrContainsReferences<TValue>()) entry.value = default!;

                  this.Dictionary.freeCount += 1;
                  this.Dictionary.freeList   = index;

                  return true;
                }

                recentIndex = index;
                index       = entry.next;

                if (++collisionCount > (uint) this.Dictionary.entries.Length)
                throw new System.InvalidOperationException("Lorem ipsum");
              }
            }

            dictionaryKey = default;
            value         = default;

            return false;
          }

          // → Attempts to add the specified key and value to the dictionary
          public bool TryAdd(TAlternateKey key, TValue value) {
            ref TValue? slot = ref this.GetValueRefOrAddDefault(key, out bool exists);

            // …
            if (!exists) {
              slot = value;
              return true;
            }

            return false;
          }

          // → Gets the value associated with the specified alternate key
          public bool TryGetValue(TAlternateKey key, /* [System.Diagnostics.CodeAnalysis.MaybeNullWhen(false)] */ out TValue value) {
            ref TValue reference = ref this.FindValue(key, out _);

            // …
            if (!System.Runtime.CompilerServices.Unsafe.IsNullRef(ref reference)) {
              value = reference;
              return true;
            }

            value = default;
            return false;
          }

          public bool TryGetValue(TAlternateKey key, /* [System.Diagnostics.CodeAnalysis.MaybeNullWhen(false)] */ out TKey dictionaryKey, /* [System.Diagnostics.CodeAnalysis.MaybeNullWhen(false)] */ out TValue value) {
            ref TValue reference = ref this.FindValue(key, out dictionaryKey);

            // …
            if (!System.Runtime.CompilerServices.Unsafe.IsNullRef(ref reference)) {
              value = reference;
              return true;
            }

            value = default;
            return false;
          }

          // → Gets or sets the value associated with the specified alternate key
          public TValue this[TAlternateKey key] {
            get {
              ref TValue value = ref this.FindValue(key, out _);

              // …
              if (System.Runtime.CompilerServices.Unsafe.IsNullRef(ref value))
              throw new System.Collections.Generic.KeyNotFoundException(GetAlternateComparer(Dictionary).Create(key));

              return value;
            }

            set => this.GetValueRefOrAddDefault(key, out _) = value;
          }
        #endif
      }

      // → Helper class containing APIs exposed through `System.Collections.Generic.CollectionExtensions` or `System.Runtime.InteropServices.CollectionsMarshal`. These methods are relatively niche and only used in specific scenarios, so adding them in a separate type avoids the additional overhead on each `PatchOdyssey.Dictionary<TKey, TValue>` instantiation — especially in Ahead-Of-Time scenarios
      internal static class CollectionsMarshalHelper {
        public static ref TValue? GetValueRefOrAddDefault(Dictionary<TKey, TValue> dictionary, TKey key, out bool exists) /* → See `PatchOdyssey.Dictionary<TKey, TValue>.TryInsert(…)` */ {
          if (null == key) throw new System.ArgumentNullException("Lorem ipsum");
          if (null == dictionary.buckets) dictionary.Initialize(0);

          uint                                                collisionCount = 0u;
          System.Collections.Generic.IEqualityComparer<TKey>? comparer       = dictionary.comparer;
          uint                                                hashCode       = (uint) (typeof(TKey).IsValueType && null == comparer ? key.GetHashCode() : comparer!.GetHashCode(key));
          ref int                                             bucket         = ref dictionary.GetBucket(hashCode);
          int                                                 index          = bucket - 1; // → Value in `this.buckets` is one-based

          // …
          if (typeof(TKey).IsValueType && null == comparer)
            while ((uint) dictionary.entries.Length > (uint) index) {
              if (hashCode == dictionary.entries[index].hashCode && System.Collections.Generic.EqualityComparer<TKey>.Default.Equals(dictionary.entries[index].key, key)) {
                exists = true;
                return ref dictionary.entries[index].value!;
              }

              index = dictionary.entries[index].next;

              if (++collisionCount > (uint) dictionary.entries.Length)
              throw new System.InvalidOperationException("Lorem ipsum");
            }

          else
            while ((uint) dictionary.entries.Length > (uint) index) {
              if (hashCode == dictionary.entries[index].hashCode && comparer.Equals(dictionary.entries[index].key, key)) {
                exists = true;
                return ref dictionary.entries[index].value!;
              }

              index = dictionary.entries[index].next;

              if (++collisionCount > (uint) dictionary.entries.Length)
              throw new System.InvalidOperationException("Lorem ipsum");
            }

          // …
          if (dictionary.freeCount > 0) {
            index                 = dictionary.freeList;
            dictionary.freeCount -= 1;
            dictionary.freeList   = Dictionary<TKey, TValue>.StartOfFreeList - dictionary.entries[dictionary.freeList].next;
          }

          else {
            int count = dictionary.count;

            // …
            if (count == dictionary.entries.Length) {
              dictionary.Resize();
              bucket = ref dictionary.GetBucket(hashCode);
            }

            index            = count;
            dictionary.count = count + 1;
          }

          // …
          ref Entry entry = ref dictionary.entries![index];

          entry.value         = default!;
          entry.key           = key;
          entry.next          = bucket - 1; // → Value in `this.buckets` is one-based
          entry.hashCode      = hashCode;
          bucket              = index + 1; // → Value in `this.buckets` is one-based
          dictionary.version += 1;

          if (!typeof(TKey).IsValueType && collisionCount > Hash.HashCollisionThreshold && comparer is NonRandomizedStringEqualityComparer) {
            // → Value types never rehash
            dictionary.Resize(dictionary.entries.Length, true);
            exists = false;

            // → At this point the `this.Dictionary.entries` array has been resized, so the current reference we have is no longer valid so we're forced to do a new lookup and return an updated reference to the new entry instance. This new lookup is guaranteed to always find a value though and it will never return a null reference here
            return ref dictionary.FindValue(key)!;
          }

          exists = false;

          return ref entry.value!;
        }
      }

      private /* sealed */ struct Entry {
        public uint   hashCode;
        public TKey   key;
        public int    next; // → Zero-based index of next-chained entry; `-1` connotes the end of the chain — also encodes if the entry is part of the free list by changing sign and subtracting `StartOfFreeList`, with `-2` connoting the end of the free list, `-3` connoting index `0` on the free list, `-4` connoting index `1` on the free list, etc…
        public TValue value;
      }

      public struct Enumerator : System.Collections.Generic.IEnumerator<System.Collections.Generic.KeyValuePair<TKey, TValue>>, System.Collections.IDictionaryEnumerator {
        internal const int DictionaryEntry = 1;
        internal const int KeyValuePair    = 2;

        public           System.Collections.Generic.KeyValuePair<TKey, TValue> Current => this.current;
        private          System.Collections.Generic.KeyValuePair<TKey, TValue> current;
        private readonly PatchOdyssey.Dictionary<TKey, TValue>                 dictionary;
        private readonly int                                                   GetEnumeratorReturnType; // → What should `Enumerator.Current` return?
        private          int                                                   index;
        private readonly int                                                   version;

        object? System.Collections.IEnumerator.Current { get {
          if (0 == this.index || this.index == this.dictionary.count + 1)
          throw new System.InvalidOperationException("Lorem ipsum");

          return Enumerator.DictionaryEntry == this.GetEnumeratorReturnType ? (object) new System.Collections.DictionaryEntry(this.current.Key, this.current.Value) : (object) new System.Collections.Generic.KeyValuePair<TKey, TValue>(this.current.Key, this.current.Value);
        } }

        System.Collections.DictionaryEntry System.Collections.IDictionaryEnumerator.Entry { get {
          if (this.index == 0 || this.index == this.dictionary.count + 1)
          throw new System.InvalidOperationException("Lorem ipsum");

          return new System.Collections.DictionaryEntry(this.current.Key, this.current.Value);
        } }

        object System.Collections.IDictionaryEnumerator.Key { get {
          if (this.index == 0 || this.index == this.dictionary.count + 1)
          throw new System.InvalidOperationException("Lorem ipsum");

          return this.current.Key;
        } }

        object? System.Collections.IDictionaryEnumerator.Value { get {
          if (this.index == 0 || this.index == this.dictionary.count + 1)
          throw new System.InvalidOperationException("Lorem ipsum");

          return this.current.Value;
        } }

        /* … */
        internal Enumerator(PatchOdyssey.Dictionary<TKey, TValue> dictionary, int returnType) {
          this.current                 = default;
          this.dictionary              = dictionary;
          this.GetEnumeratorReturnType = returnType;
          this.index                   = 0;
          this.version                 = dictionary.version;
        }

        /* … */
        public void Dispose() {}

        public bool MoveNext() {
          if (this.dictionary.version != this.version)
          throw new System.InvalidOperationException("Lorem ipsum");

          while ((uint) this.dictionary.count > (uint) this.index) {
            ref Entry entry = ref this.dictionary.entries![this.index++];

            // …
            if (entry.next >= -1) {
              this.current = new(entry.key, entry.value);
              return true;
            }
          }

          this.current = default;
          this.index   = this.dictionary.count + 1;

          return false;
        }

        void System.Collections.IEnumerator.Reset() {
          if (this.dictionary.version != this.version)
            throw new System.InvalidOperationException("Lorem ipsum");

          this.current = default;
          this.index   = 0;
        }
      }

      public class GenericEqualityComparer<T> : System.Collections.Generic.IEqualityComparer<T> {
        public static GenericEqualityComparer<T> Create() {
          return new();
        }

        // → Determines whether the specified objects are equal
        public bool Equals(T a, T b) {
          return GenericEqualityComparer.Create().Equals(a, b);
        }

        // → Returns a hash code for the specified object
        public int GetHashCode(T _) {
          return 0;
        }
      }

      public class GenericEqualityComparer : System.Collections.IEqualityComparer {
        public static GenericEqualityComparer Create() {
          return new();
        }

        // → Determines whether the specified objects are equal
        public new bool Equals(object a, object b) {
          (System.Type a, System.Type b) types = (a.GetType(), b.GetType());

          // …
          foreach (System.Reflection.PropertyInfo property in types.a.GetProperties()) {
            (System.Reflection.PropertyInfo a, System.Reflection.PropertyInfo? b) properties = (property,                       types.b.GetProperty(property.Name));
            (object?                        a, object?                         b) values     = (properties.a.GetValue(a, null), properties.b?.GetValue(b, null));

            // …
            if (null == properties.b)                 return false;
            if (null == values.a && null == values.b) continue;
            if (null == values.a || null == values.b) return false;
            if (!values.a.Equals(values.b))           return false;
          }

          foreach (System.Reflection.FieldInfo field in types.a.GetFields()) {
            (System.Reflection.FieldInfo a, System.Reflection.FieldInfo b) fields = (field,                types.b.GetField(field.Name));
            (object?                     a, object?                     b) values = (fields.a.GetValue(a), fields.b?.GetValue(b));

            // …
            if (null == fields.b)                     return false;
            if (null == values.a && null == values.b) continue;
            if (null == values.a || null == values.b) return false;
            if (!values.a.Equals(values.b))           return false;
          }

          return true;
        }

        // → Returns a hash code for the specified object
        public int GetHashCode(object _) {
          return 0;
        }
      }

      internal static class Hash {
        // → Typical resize algorithms would pick the smallest prime number in this array that is larger than twice the previous capacity.
        // → Doubling is important for preserving the asymptotic complexity of the hash table operations such as `::Add(…)`.
        // → Having a prime guarantees that double hashing does not lead to infinite loops
        internal static readonly System.Collections.ObjectModel.ReadOnlyCollection<int> Primes                 = System.Array.AsReadOnly(new[] {3, 7, 11, 17, 23, 29, 37, 47, 59, 71, 89, 107, 131, 163, 197, 239, 293, 353, 431, 521, 631, 761, 919, 1103, 1327, 1597, 1931, 2333, 2801, 3371, 4049, 4861, 5839, 7013, 8419, 10103, 12143, 14591, 17519, 21023, 25229, 30293, 36353, 43627, 52361, 62851, 75431, 90523, 108631, 130363, 156437, 187751, 225307, 270371, 324449, 389357, 467237, 560689, 672827, 807403, 968897, 1162687, 1395263, 1674319, 2009191, 2411033, 2893249, 3471899, 4166287, 4999559, 5999471, 7199369, 8639249, 10367101, 12440537, 14928671, 17914409, 21497293, 25796759, 30956117, 37147349, 44576837, 53492207, 64190669, 77028803, 92434613, 110921543, 133105859, 159727031, 191672443, 230006941, 276008387, 331210079, 397452101, 476942527, 572331049, 686797261, 824156741, 988988137, 1186785773, 1424142949, 1708971541, 2050765853, Hash.MaxPrimeArrayLength});
        public const int                                                                MaxPrimeArrayLength    = 0x7FFFFFC3; // → `0x7FEFFFFD`; Lesser than `System.Array.MaxLength`
        public const int                                                                HashPrime              = 101;
        public const uint                                                               HashCollisionThreshold = 100;

        /* … */
        public static int ExpandPrime(int size) {
          int capacity = size * 2; // → Allow the hash tables to grow to maximum possible size before encountering capacity overflow.
          return Hash.MaxPrimeArrayLength < (uint) capacity && Hash.MaxPrimeArrayLength > size ? Hash.MaxPrimeArrayLength : Hash.GetPrime(capacity);
        }

        // → Performs a mod operation using the multiplier pre-computed with `Hash.GetFastModMultiplier(…)`; This should only be used on 64-bit
        [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)]
        public static uint FastMod(uint value, uint divisor, ulong multiplier) /* → `System.Math.BigMul(multiplier * value, divisor, out _)` */ {
          // → We use modified Daniel Lemire's fast modulus algorithm (`https://github.com/dotnet/runtime/pull/406`), which allows to avoid the long multiplication if the divisor is less than `2 ** 31`
          return (uint) (((((multiplier * value) >> 32) + 1) * divisor) >> 32);
        }

        // → Returns approximate reciprocal of the divisor; This should only be used on 64-bit
        public static ulong GetFastModMultiplier(uint divisor) {
          return ulong.MaxValue / divisor + 1;
        }

        public static int GetPrime(int minimum) {
          if (minimum < 0)
          throw new System.ArgumentException("Lorem ipsum");

          foreach (int prime in Hash.Primes) {
            if (minimum <= prime)
            return prime;
          }

          // → Outside of our predefined table. Compute the hard way.
          for (int index = minimum | 1; index < int.MaxValue; index += 2) {
            if (Hash.IsPrime(index) && 0 != (index - 1) % HashPrime)
            return index;
          }

          return minimum;
        }

        public static bool IsPrime(int candidate) {
          if ((candidate & 1) != 0) {
            int limit = (int) System.Math.Sqrt(candidate);

            // …
            for (int divisor = 3; divisor <= limit; divisor += 2) {
              if (0 == (candidate % divisor))
              return false;
            }

            return true;
          }

          return candidate == 2;
        }
      }

      enum InsertionBehavior : byte {
        ThrowOnExisting
      }

      [System.Diagnostics.DebuggerDisplay("Count = {Count}")]
      public sealed class KeyCollection : System.Collections.Generic.ICollection<TKey>, System.Collections.ICollection, System.Collections.Generic.IReadOnlyCollection<TKey> {
        public struct Enumerator : System.Collections.Generic.IEnumerator<TKey>, System.Collections.IEnumerator {
          public                          TKey                                  Current => this.currentKey!;
          private /* required */          TKey?                                 currentKey;
          private /* required */ readonly PatchOdyssey.Dictionary<TKey, TValue> dictionary;
          private /* required */          int                                   index;
          private /* required */ readonly int                                   version;
          object?                                                               System.Collections.IEnumerator.Current { get {
            if (0 == this.index || this.index == this.dictionary.count + 1)
              throw new System.InvalidOperationException("Lorem ipsum");

            return this.currentKey;
          } }

          /* … */
          internal Enumerator(PatchOdyssey.Dictionary<TKey, TValue> dictionary) {
            this.currentKey = default;
            this.dictionary = dictionary;
            this.index      = 0;
            this.version    = dictionary.version;
          }

          /* … */
          public void Dispose() {}

          public bool MoveNext() {
            if (this.dictionary.version != this.version)
            throw new System.InvalidOperationException("Lorem ipsum");

            while ((uint) this.dictionary.Count > (uint) this.index) {
              ref Entry entry = ref this.dictionary.entries![this.index++];

              if (entry.next >= -1) {
                this.currentKey = entry.key;
                return true;
              }
            }


            this.currentKey = default;
            this.index      = this.dictionary.count + 1;

            return false;
          }

          void System.Collections.IEnumerator.Reset() {
            if (this.dictionary.version != this.version)
            throw new System.InvalidOperationException("Lorem ipsum");

            this.currentKey = default;
            this.index      = 0;
          }
        }

        /* … */
        public                          int                                   Count => this.dictionary.Count;
        private /* required */ readonly PatchOdyssey.Dictionary<TKey, TValue> dictionary;
        bool                                                                  System.Collections.Generic.ICollection<TKey>.IsReadOnly     => true;
        bool                                                                  System.Collections.ICollection              .IsSynchronized => false;
        object                                                                System.Collections.ICollection              .SyncRoot       => ((System.Collections.ICollection) this.dictionary).SyncRoot;

        /* … */
        public KeyCollection(PatchOdyssey.Dictionary<TKey, TValue> dictionary) {
          if (null == dictionary)
          throw new System.ArgumentNullException("Lorem ipsum");

          this.dictionary = dictionary;
        }

        /* … */
        public bool Contains(TKey item) {
          return this.dictionary.ContainsKey(item);
        }

        public void CopyTo(TKey[] array, int index) {
          int      count   = this.dictionary.count;
          Entry[]? entries = this.dictionary.entries;

          // …
          if (null == array)                                throw new System.ArgumentNullException      ("Lorem ipsum");
          if (index < 0 || index > array.Length)            throw new System.ArgumentOutOfRangeException("Lorem ipsum");
          if (this.dictionary.Count > array.Length - index) throw new System.ArgumentException          ("Lorem ipsum");

          for (int subindex = 0; subindex != this.dictionary.count; ++subindex) {
            if (this.dictionary.entries![subindex].next >= -1)
            array[index++] = this.dictionary.entries[subindex].key;
          }
        }

        public Enumerator GetEnumerator() {
          new(this.dictionary);
        }

        void System.Collections.ICollection.CopyTo(System.Array array, int index) {
          if (null == array)                                throw new System.ArgumentNullException      ("Lorem ipsum");
          if (array.Rank != 1)                              throw new System.ArgumentException          ("Lorem ipsum");
          if (0 != array.GetLowerBound(0))                  throw new System.ArgumentException          ("Lorem ipsum");
          if ((uint) array.Length < (uint) index)           throw new System.ArgumentOutOfRangeException("Lorem ipsum");
          if (this.dictionary.Count > array.Length - index) throw new System.ArgumentException          ("Lorem ipsum");

          if (array is TKey[] keys) this.CopyTo(keys, index);
          else {
            object[]? objects = array as object[];

            // …
            if (null == objects)
            throw new System.ArgumentException("Lorem ipsum");

            try {
              for (int subindex = 0; subindex != this.dictionary.count; ++subindex) {
                if (this.dictionary.entries![subindex].next >= -1)
                objects[index++] = this.dictionary.entries[subindex].key;
              }
            } catch (System.ArrayTypeMismatchException) { throw new System.ArgumentException("Lorem ipsum"); }
          }
        }

        void System.Collections.Generic.ICollection<TKey>.Add(TKey item) {
          throw new System.NotSupportedException("Lorem ipsum");
        }

        void System.Collections.Generic.ICollection<TKey>.Clear() {
          throw new System.NotSupportedException("Lorem ipsum");
        }

        bool System.Collections.Generic.ICollection<TKey>.Remove(TKey item) {
          throw new System.NotSupportedException("Lorem ipsum");
          return false;
        }

        System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator() {
          return ((System.Collections.Generic.IEnumerable<TKey>) this).GetEnumerator();
        }

        System.Collections.Generic.IEnumerator<TKey> System.Collections.Generic.IEnumerable<TKey>.GetEnumerator() {
          return 0 == this.Count ? new TKey[0].GetEnumerator() : this.GetEnumerator();
        }
      }

      [System.Serializable]
      public sealed class NonRandomizedStringEqualityComparer : System.Collections.Generic.EqualityComparer<string?>, System.Runtime.Serialization.ISerializable {
        internal static new System.Collections.Generic.IEqualityComparer<string?> Default { get; } = new NonRandomizedStringEqualityComparer();

        /* … */
        private NonRandomizedStringEqualityComparer() {}
        private NonRandomizedStringEqualityComparer(System.Runtime.Serialization.SerializationInfo information, System.Runtime.Serialization.StreamingContext context) {}

        /* … */
        public sealed override bool Equals(string? stringA, string? stringB) {
          return string.Equals(stringA, stringB);
        }

        public sealed override int GetHashCode(string? _) {
          return _?.GetNonRandomizedHashCode() ?? 0;
        }

        public void GetObjectData(System.Runtime.Serialization.SerializationInfo information, System.Runtime.Serialization.StreamingContext context) {
          information.SetType(typeof(GenericEqualityComparer<string>));
        }
      }

      [System.Diagnostics.DebuggerDisplay("Count = {Count}")]
      public sealed class ValueCollection : System.Collections.Generic.ICollection<TValue>, System.Collections.ICollection, System.Collections.Generic.IReadOnlyCollection<TValue> {
        public struct Enumerator : System.Collections.Generic.IEnumerator<TValue>, System.Collections.IEnumerator {
          public           TValue                                Current => this.current!;
          private          TValue?                               current;
          private readonly PatchOdyssey.Dictionary<TKey, TValue> dictionary;
          private          int                                   index;
          private readonly int                                   version;
          object?                                                System.Collections.IEnumerator.Current { get {
            if (this.index == 0 || this.index == this.dictionary.count + 1)
              throw new System.InvalidOperationException("Lorem ipsum");

            return this.current;
          } }

          /* … */
          internal Enumerator(Dictionary<TKey, TValue> dictionary) {
            this.current    = default;
            this.dictionary = dictionary;
            this.index      = 0;
            this.version    = dictionary.version;
          }

          /* … */
          public void Dispose() {}

          public bool MoveNext() {
            if (this.dictionary.version != this.version)
            throw new System.InvalidOperationException("Lorem ipsum");

            while ((uint) this.dictionary.count > (uint) this.index) {
              ref Entry entry = ref this.dictionary.entries![this.index++];

              if (entry.next >= -1) {
                this.current = entry.value;
                return true;
              }
            }

            this.current = default;
            this.index   = this.dictionary.count + 1;

            return false;
          }

          void System.Collections.IEnumerator.Reset() {
            if (this.dictionary.version != this.version)
            throw new System.InvalidOperationException("Lorem ipsum");

            this.current = default;
            this.index   = 0;
          }
        }

        /* … */
        public                          int                                   Count => this.dictionary.Count;
        private /* required */ readonly PatchOdyssey.Dictionary<TKey, TValue> dictionary;
        bool                                                                  System.Collections.ICollection                .IsSynchronized => false;
        object                                                                System.Collections.ICollection                .SyncRoot       => ((System.Collections.ICollection) this.dictionary).SyncRoot;
        bool                                                                  System.Collections.Generic.ICollection<TValue>.IsReadOnly     => true;

        /* … */
        public ValueCollection(Dictionary<TKey, TValue> dictionary) {
          if (null == dictionary)
          throw new System.ArgumentNullException("Lorem ipsum");

          this.dictionary = dictionary;
        }

        /* … */
        public void CopyTo(TValue[] array, int index) {
          if (null == array)                                throw new System.ArgumentNullException      ("Lorem ipsum");
          if (array.Length < (uint) index)                  throw new System.ArgumentOutOfRangeException("Lorem ipsum");
          if (this.dictionary.Count > array.Length - index) throw new System.ArgumentException          ("Lorem ipsum");

          for (int subindex = 0; subindex != this.dictionary.Count; ++subindex) {
            if (this.dictionary.entries![subindex].next >= -1)
            array[index++] = this.dictionary.entries[subindex].value;
          }
        }

        public Enumerator GetEnumerator() {
          return new(this.dictionary);
        }

        void System.Collections.Generic.ICollection<TValue>.Add(TValue item) {
          throw new System.NotSupportedException("Lorem ipsum");
        }

        void System.Collections.Generic.ICollection<TValue>.Clear() {
          throw new System.NotSupportedException("Lorem ipsum");
        }

        bool System.Collections.Generic.ICollection<TValue>.Contains(TValue item) {
          return this.dictionary.ContainsValue(item);
        }

        void System.Collections.ICollection.CopyTo(System.Array array, int index) {
          if (null == array)                                throw new System.ArgumentNullException      ("Lorem ipsum");
          if (array.Rank != 1)                              throw new System.ArgumentException          ("Lorem ipsum");
          if (0 != array.GetLowerBound(0))                  throw new System.ArgumentException          ("Lorem ipsum");
          if ((uint) array.Length < (uint) index)           throw new System.ArgumentOutOfRangeException("Lorem ipsum");
          if (this.dictionary.Count > array.Length - index) throw new System.ArgumentException          ("Lorem ipsum");

          if (array is TValue[] values) this.CopyTo(values, index);
          else {
            object[]? objects = array as object[];

            // …
            if (null == objects)
            throw new System.ArgumentException("Lorem ipsum");

            try {
              for (int subindex = 0; subindex != this.dictionary.Count; ++subindex) {
                if (this.dictionary.entries![subindex].next >= -1)
                objects[index++] = this.dictionary.entries[subindex].value!;
              }
            } catch (System.ArrayTypeMismatchException) { throw new System.ArgumentException("Lorem ipsum"); }
          }
        }

        bool System.Collections.Generic.ICollection<TValue>.Remove(TValue item) {
          throw new System.NotSupportedException("Lorem ipsum");
          return false;
        }

        System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator() {
          return ((System.Collections.Generic.IEnumerable<TValue>) this).GetEnumerator();
        }

        System.Collections.Generic.IEnumerator<TValue> System.Collections.Generic.IEnumerable<TValue>.GetEnumerator() {
          return 0 == this.Count ? new TValue[0].GetEnumerator() : this.GetEnumerator();
        }
      }

      /* … */
      private const string ComparerName      = "Comparer";      // → Do not rename (binary serialization)
      private const string HashSizeName      = "HashSize";      // → Do not rename (binary serialization); Must save buckets.Length
      private const string KeyValuePairsName = "KeyValuePairs"; // → Do not rename (binary serialization)
      private const int    StartOfFreeList   = -3;              //
      private const string VersionName       = "Version";       // → Do not rename (binary serialization)

      private int[]?                                              buckets;
      public  int                                                 Capacity => this.entries?.Length ?? 0;
      private int                                                 count;
      public  System.Collections.Generic.IEqualityComparer<TKey>  Comparer => this.comparer ?? System.Collections.Generic.EqualityComparer<TKey>.Default;
      private System.Collections.Generic.IEqualityComparer<TKey>? comparer;
      public  int                                                 Count => this.count - this.freeCount;
      private Entry[]?                                            entries;
      private int                                                 freeCount;
      private int                                                 freeList;
      private int                                                 version;
      public  KeyCollection                                       Keys => this.keys ??= new KeyCollection(this);
      private KeyCollection?                                      keys;
      public  ValueCollection                                     Values => this.values ??= new ValueCollection(this);
      private ValueCollection?                                    values;
      bool                                                        System.Collections.        IDictionary                                                                .IsFixedSize    => false;
      bool                                                        System.Collections.        IDictionary                                                                .IsReadOnly     => false;
      bool                                                        System.Collections.        ICollection                                                                .IsSynchronized => false;
      object                                                      System.Collections.        ICollection                                                                .SyncRoot       => this;
      System.Collections.ICollection                              System.Collections.        IDictionary                                                                .Keys          => Keys;
      System.Collections.ICollection                              System.Collections.        IDictionary                                                                .Values        => Values;
      bool                                                        System.Collections.Generic.ICollection        <System.Collections.Generic.KeyValuePair<TKey, TValue>>.IsReadOnly     => false;
      System.Collections.Generic.ICollection<TKey>                System.Collections.Generic.IDictionary        <TKey, TValue>                                         .Keys           => Keys;
      System.Collections.Generic.ICollection<TValue>              System.Collections.Generic.IDictionary        <TKey, TValue>                                         .Values         => Values;
      System.Collections.Generic.IEnumerable<TKey>                System.Collections.Generic.IReadOnlyDictionary<TKey, TValue>                                         .Keys           => Keys;
      System.Collections.Generic.IEnumerable<TValue>              System.Collections.Generic.IReadOnlyDictionary<TKey, TValue>                                         .Values         => Values;
      #if TARGET_64BIT
        private ulong fastModMultiplier;
      #endif

      /* … */
      public Dictionary()                                                                                                         : this(0,          null)     {}
      public Dictionary(int                                                                                           capacity)   : this(capacity,   null)     {}
      public Dictionary(System.Collections.Generic.IEqualityComparer<TKey>?                                           comparer)   : this(0,          comparer) {}
      public Dictionary(System.Collections.Generic.IDictionary<TKey, TValue>                                          dictionary) : this(dictionary, null)     {}
      public Dictionary(System.Collections.Generic.IEnumerable<System.Collections.Generic.KeyValuePair<TKey, TValue>> collection) : this(collection, null)     {}

      public Dictionary(int capacity, System.Collections.Generic.IEqualityComparer<TKey>? comparer) {
        if (capacity < 0) throw new System.ArgumentOutOfRangeException("Lorem ipsum");
        if (capacity > 0) this.Initialize(capacity);

        // → For reference types, we always want to store a comparer instance, either the one provided or the default. For value types, the default is provided or `EqualityComparer<TKey>.Default.Equals` on every use, enabling the JIT to devirtualize and possibly inline the operation
        if (!typeof(TKey).IsValueType) {
          this.comparer = comparer ?? System.Collections.Generic.EqualityComparer<TKey>.Default;

          // → Special-case `System.Collections.Generic.EqualityComparer<string>.Default`, `System.StringComparer.Ordinal`, and `System.StringComparer.OrdinalIgnoreCase`: We use a non-randomized comparer for improved performance, falling back to a randomized comparer if the hash buckets become unbalanced
          if (typeof(TKey) == typeof(string) && NonRandomizedStringEqualityComparer.GetStringComparer(this.comparer!) is System.Collections.Generic.IEqualityComparer<string> stringComparer)
          this.comparer = (System.Collections.Generic.IEqualityComparer<TKey>) stringComparer;
        }

        else if (comparer is not null && comparer != System.Collections.Generic.EqualityComparer<TKey>.Default)
          this.comparer = comparer; // → Initial check for `null` to avoid forcing default comparer instantiation unnecessarily
      }

      public Dictionary(System.Collections.Generic.IDictionary<TKey, TValue> dictionary, System.Collections.Generic.IEqualityComparer<TKey>? comparer) : this(dictionary?.Count ?? 0, comparer) {
        if (null == dictionary)
        throw new System.ArgumentNullException("Lorem ipsum");

        this.AddRange(dictionary);
      }

      public Dictionary(System.Collections.Generic.IEnumerable<System.Collections.Generic.KeyValuePair<TKey, TValue>> collection, System.Collections.Generic.IEqualityComparer<TKey>? comparer) : this((collection as System.Collections.Generic.ICollection<System.Collections.Generic.KeyValuePair<TKey, TValue>>)?.Count ?? 0, comparer) {
        if (null == collection)
          throw new System.ArgumentNullException("Lorem ipsum");

        this.AddRange(collection);
      }

      /* … */
      public void Add(TKey key, TValue value) {
        this.TryInsert(key, value, InsertionBehavior.ThrowOnExisting);
      }

      private void AddRange(System.Collections.Generic.IEnumerable<System.Collections.Generic.KeyValuePair<TKey, TValue>> enumerable) {
        // → It is likely that the passed-in enumerable is `PatchOdyssey.Dictionary<TKey,TValue>`, when this is the case, avoid the enumerator allocation and overhead by looping through the entries array directly. This is done to maintain backward-compatibility with subclasses that may have overridden the enumerator behavior
        if (enumerable.GetType() == typeof(PatchOdyssey.Dictionary<TKey, TValue>)) {
          PatchOdyssey.Dictionary<TKey, TValue> source  = (PatchOdyssey.Dictionary<TKey, TValue>) enumerable;
          Entry[]                               entries = source.entries;

          // …
          if (0 == source.Count)
          return;

          if (source.comparer == this.comparer) {
            this.CopyEntries(entries, source.count);
            return;
          }

          for (int index = 0; index != source.count; ++index) {
            if (entries[index].next >= -1)
            this.Add(entries[index].key, entries[index].value);
          }

          return;
        }

        foreach (System.Collections.Generic.KeyValuePair<TKey, TValue> pair in enumerable)
        this.Add(pair.Key, pair.Value);
      }

      public void Clear() {
        int count = this.count;

        if (count > 0) {
          this.count     = 0;
          this.freeCount = 0;
          this.freeList  = -1;

          System.Array.Clear(this.buckets);
          System.Array.Clear(this.entries, 0, count);
        }
      }

      public bool ContainsKey(TKey key) {
        return !System.Runtime.CompilerServices.Unsafe.IsNullRef(ref this.FindValue(key));
      }

      public bool ContainsValue(TValue value) {
        if (null == value) {
          for (int index = 0; index != this.count; ++index) {
            if (this.entries![index].next >= -1 && null == this.entries[index].value)
            return true;
          }
        }

        else if (typeof(TValue).IsValueType) {
          for (int index = 0; index != this.count; ++index) {
            if (this.entries![index].next >= -1 && System.Collections.Generic.EqualityComparer<TValue>.Default.Equals(this.entries[index].value, value))
            return true;
          }
        }

        else {
          for ((System.Collections.Generic.EqualityComparer<TValue> comparer, int index) = (System.Collections.Generic.EqualityComparer<TValue>.Default, 0); index != this.count; ++index) {
            if (this.entries![index].next >= -1 && comparer.Equals(this.entries[index].value, value))
            return true;
          }
        }

        return false;
      }

      private void CopyEntries(Entry[] entries, int count) {
        Entry[] entries  = this.entries;
        int     subcount = 0;

        // …
        for (int index = 0; count != index; ++index) {
          uint hashCode = entries[index].hashCode;

          // …
          if (entries[index].next >= -1) {
            ref int   bucket = ref this.GetBucket(hashCode);
            ref Entry entry  = ref entries[subcount];

            // …
            entry      = entries[index];
            entry.next = bucket - 1; // → Value in `this.buckets` is one-based
            bucket     = subcount++ + 1;
          }
        }

        this.count     = subcount;
        this.freeCount = 0;
      }

      private void CopyTo(System.Collections.Generic.KeyValuePair<TKey, TValue>[] array, int index) {
        if (null == array)                      throw new System.ArgumentNullException   ("Lorem ipsum");
        if ((uint) array.Length < (uint) index) throw new System.IndexOutOfRangeException("Lorem ipsum");
        if (this.Count > array.Length - index)  throw new System.ArgumentException       ("Lorem ipsum");

        for (int subindex = 0; subindex != this.count; ++subindex) {
          if (this.entries![i].next >= -1)
          array[index++] = new System.Collections.Generic.KeyValuePair<TKey, TValue>(this.entries[i].key, this.entries[i].value);
        }
      }

      // → Ensures that the dictionary can hold up to `capacity` entries without any further expansion of its backing storage
      public int EnsureCapacity(int capacity) {
        int dictionaryCapacity = this.entries?.Length ?? 0;

        // …
        if (capacity < 0)                   throw new System.ArgumentOutOfRangeException("Lorem ipsum");
        if (capacity <= dictionaryCapacity) return dictionaryCapacity;

        ++this.version;

        if (null == this.buckets)
        return this.Initialize(capacity);

        capacity = Hash.GetPrime(capacity);
        this.Resize(capacity, rehash: false);

        return capacity;
      }

      internal ref TValue FindValue(TKey key) {
        if (null == key) throw new System.ArgumentNullException("Lorem ipsum");
        System.Collections.Generic.IEqualityComparer<TKey>? comparer = this.comparer;
        ref Entry                                           entry    = ref System.Runtime.CompilerServices.Unsafe.NullRef<Entry>();

        // …
        if (null != this.buckets) {
          if (typeof(TKey).IsValueType && null == comparer) {
            uint     collisionCount = 0;
            Entry[]? entries        = this.entries;
            uint     hashCode       = (uint) key.GetHashCode();
            int      index          = this.GetBucket(hashCode) - 1; // → Value in `this.buckets` is one-based; subtract one from `index`

            // …
            do {
              if ((uint) entries.Length <= (uint) index)
              goto ReturnNotFound;

              entry = ref entries[index];

              if (entry.hashCode == hashCode && System.Collections.Generic.EqualityComparer<TKey>.Default.Equals(entry.key, key))
              goto ReturnFound;

              index = entry.next;
            } while (++collisionCount <= (uint) entries.Length);

            goto ConcurrentOperation;
          }

          else {
            uint     collisionCount = 0;
            Entry[]? entries        = this.entries;
            uint     hashCode       = (uint) comparer.GetHashCode(key);
            int      index          = this.GetBucket(hashCode) - 1; // → Value in `this.buckets` is one-based; subtract one from `index`

            // …
            do {
              if ((uint) entries.Length <= (uint) index)
              goto ReturnNotFound;

              entry = ref entries[index];

              if (entry.hashCode == hashCode && comparer.Equals(entry.key, key))
              goto ReturnFound;

              index = entry.next;
            } while (++collisionCount <= (uint) entries.Length);

            goto ConcurrentOperation;
          }
        }

        goto ReturnNotFound;

        ConcurrentOperation:
          throw new System.InvalidOperationException("Lorem ipsum");

        ReturnFound:
          ref TValue value = ref entry.value;

        Return:
          return ref value;

        ReturnNotFound:
          value = ref System.Runtime.CompilerServices.Unsafe.NullRef<TValue>();
          goto Return;
      }

      // → Gets an instance of a type that may be used to perform operations on the current `PatchOdyssey.Dictionary<TKey, TValue>` using a `TAlternateKey` as a key instead of a `TKey`
      public AlternateLookup<TAlternateKey> GetAlternateLookup<TAlternateKey>() where TAlternateKey : notnull /* allows ref struct */ {
        if (!AlternateLookup<TAlternateKey>.IsCompatibleKey(this))
        throw new System.InvalidOperationException("Lorem ipsum");

        return new(this);
      }

      [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)]
      private ref int GetBucket(uint hashCode) {
        #if TARGET_64BIT
          return ref this.buckets![Hash.FastMod(hashCode, (uint) this.buckets!.Length, this.fastModMultiplier)];
        #else
          return ref this.buckets![(uint) hashCode % this.buckets!.Length];
        #endif
      }

      public Enumerator GetEnumerator() {
        return new(this, Enumerator.KeyValuePair);
      }

      [System.ComponentModel.EditorBrowsable(System.ComponentModel.EditorBrowsableState.Never)]
      public virtual void GetObjectData(System.Runtime.Serialization.SerializationInfo information, System.Runtime.Serialization.StreamingContext context) {
        if (null == information)
        throw new System.ArgumentNullException("Lorem ipsum");

        information.AddValue(Dictionary<TKey, TValue>.VersionName,  this.version);
        information.AddValue(Dictionary<TKey, TValue>.ComparerName, this.Comparer, typeof(System.Collections.Generic.IEqualityComparer<TKey>));
        information.AddValue(Dictionary<TKey, TValue>.HashSizeName, this.buckets?.Length ?? 0);

        if (null != this.buckets) {
          System.Collections.Generic.KeyValuePair<TKey, TValue>[] array = new System.Collections.Generic.KeyValuePair<TKey, TValue>[this.Count];

          // …
          this.CopyTo(array, 0);
          information.AddValue(Dictionary<TKey, Tvalue>.KeyValuePairsName, array, typeof(System.Collections.Generic.KeyValuePair<TKey, TValue>[]));
        }
      }

      private int Initialize(int capacity) {
        int     size    = Hash.GetPrime(capacity);
        int[]   buckets = new int  [size];
        Entry[] entries = new Entry[size];

        // … → Allocate both arrays initially to guard against Out-of-Memory exceptions
        this.buckets  = buckets;
        this.entries  = entries;
        this.freeList = -1;
        #if TARGET_64BIT
          this.fastModMultiplier = Hash.GetFastModMultiplier((uint) size);
        #endif

        return size;
      }

      private static bool IsCompatibleKey(object key) {
        if (null == key)
        throw new System.ArgumentNullException("Lorem ipsum");

        return key is TKey;
      }

      public virtual void OnDeserialization(object? sender) {
        Hash.SerializationInfoTable.TryGetValue(this, out System.Runtime.Serialization.SerializationInfo? serialization);

        if (null != serialization) {
          int hashSize = serialization.GetInt32(Dictionary<TKey, TValue>.HashSizeName);
          int version  = serialization.GetInt32(Dictionary<TKey, TValue>.VersionName);

          // …
          this.comparer = (System.Collections.Generic.IEqualityComparer<TKey>) serialization.GetValue(Dictionary<TKey, TValue>.ComparerName, typeof(System.Collections.Generic.IEqualityComparer<TKey>))!;

          if (0 == hashSize)
            this.buckets = null;

          else {
            System.Collections.Generic.KeyValuePair<TKey, TValue>[]? array = (System.Collections.Generic.KeyValuePair<TKey, TValue>[]?) serialization.GetValue(Dictionary<TKey, Tvalue>.KeyValuePairsName, typeof(System.Collections.Generic.KeyValuePair<TKey, TValue>[]));

            // …
            this.Initialize(hashSize);

            if (null == array)
            throw new System.Runtime.Serialization.SerializationException("Lorem ipsum");

            foreach (System.Collections.Generic.KeyValuePair<TKey, TValue> pair in array) {
              if (null == pair.Key)
                throw new System.Runtime.Serialization.SerializationException("Lorem ipsum");

              this.Add(pair.Key, pair.Value);
            }
          }

          this.version = version;
          Hash.SerializationInfoTable.Remove(this);
        }
      }

      public bool Remove(TKey key) {
        if (null == key)
        throw new System.ArgumentNullException("Lorem ipsum");

        if (null != this.buckets) {
          uint                                                collisionCount = 0u;
          System.Collections.Generic.IEqualityComparer<TKey>? comparer       = this.comparer;
          uint                                                hashCode       = (uint) (typeof(TKey).IsValueType && null == comparer ? key.GetHashCode() : comparer!.GetHashCode(key));
          ref int                                             bucket         = ref this.GetBucket(hashCode);
          int                                                 index          = bucket - 1; // → Value in `this.buckets` is one-based
          int                                                 recentIndex    = -1;

          // …
          while (index >= 0) {
            ref Entry entry = ref this.entries[index];

            // …
            if (entry.hashCode == hashCode && (typeof(TKey).IsValueType && null == comparer ? System.Collections.Generic.EqualityComparer<TKey>.Default.Equals(entry.key, key) : comparer!.Equals(entry.key, key))) {
              if (recentIndex < 0) bucket = entry.next + 1; // → Value in `this.buckets` is one-based
              else this.entries[recentIndex].next = entry.next;

              entry.next = Dictionary<TKey, TValue>.StartOfFreeList - this.freeList;

              if (System.Runtime.CompilerServices.RuntimeHelpers.IsReferenceOrContainsReferences<TKey>())   entry.key   = default!;
              if (System.Runtime.CompilerServices.RuntimeHelpers.IsReferenceOrContainsReferences<TValue>()) entry.value = default!;

              this.freeCount += 1;
              this.freeList   = index;

              return true;
            }

            recentIndex = index;
            index       = entry.next;

            if (++collisionCount > (uint)this.entries.Length)
            throw new System.InvalidOperationException("Lorem ipsum");
          }
        }

        return false;
      }

      public bool Remove(TKey key, /* [System.Diagnostics.CodeAnalysis.MaybeNullWhen(false)] */ out TValue value) {
        if (null == key)
        throw new System.ArgumentException("Lorem ipsum");

        if (null != this.buckets) {
          uint                                                collisionCount = 0;
          System.Collections.Generic.IEqualityComparer<TKey>? comparer       = this.comparer;
          uint                                                hashCode       = (uint) (typeof(TKey).IsValueType && null == comparer ? key.GetHashCode() : comparer!.GetHashCode(key));
          ref int                                             bucket         = ref this.GetBucket(hashCode);
          int                                                 index          = bucket - 1; // → Value in `this.buckets` is one-based
          int                                                 recentIndex    = -1;

          // …
          while (index >= 0) {
            ref Entry entry = ref this.entries[index];

            // …
            if (entry.hashCode == hashCode && (typeof(TKey).IsValueType && null == comparer ? System.Collections.Generic.EqualityComparer<TKey>.Default.Equals(entry.key, key) : comparer!.Equals(entry.key, key))) {
              if (recentIndex < 0) bucket = entry.next + 1; // → Value in `this.buckets` is one-based
              else this.entries[recentIndex].next = entry.next;

              value       = entry.value;
              entry.next = Dictionary<TKey, TValue>.StartOfFreeList - this.freeList;

              if (System.Runtime.CompilerServices.RuntimeHelpers.IsReferenceOrContainsReferences<TKey>())   entry.key   = default!;
              if (System.Runtime.CompilerServices.RuntimeHelpers.IsReferenceOrContainsReferences<TValue>()) entry.value = default!;

              this.freeCount += 1;
              this.freeList   = index;

              return true;
            }

            recentIndex = index;
            index       = entry.next;

            if (++collisionCount > (uint)this.entries.Length)
            throw new System.InvalidOperationException("Lorem ipsum");
          }
        }

        value = default;

        return false;
      }

      private void Resize() {
        return this.Resize(Hash.ExpandPrime(this.count), false);
      }

      private void Resize(int size, bool rehash) {
        Entry[] entries = new Entry[size];

        // …
        System.Array.Copy(this.entries, entries, this.count);

        if (!typeof(TKey).IsValueType && rehash) {
          System.Collections.Generic.IEqualityComparer<TKey> comparer = this.comparer = (System.Collections.Generic.IEqualityComparer<TKey>) ((NonRandomizedStringEqualityComparer) this.comparer).GetRandomizedEqualityComparer();

          // …
          for (int index = 0; index != this.count; ++index) {
            if (entries[index].next >= -1)
            entries[index].hashCode = (uint) comparer.GetHashCode(entries[index].key);
          }
        }

        // Assign member variables after both arrays allocated to guard against corruption from OOM if second fails
        this.buckets = new int[size];
        #if TARGET_64BIT
          this.fastModMultiplier = Hash.GetFastModMultiplier((uint) size);
        #endif

        for (int index = 0; index != this.count; ++index)
        if (entries[index].next >= -1) {
          ref int bucket = ref this.GetBucket(entries[index].hashCode);

          // …
          entries[index].next = bucket - 1; // → Value in `this.buckets` is one-based
          bucket              = index  + 1;
        }

        this.entries = entries;
      }

      // → Sets the capacity of this dictionary to what it would be if it had been originally initialized with all its entries
      public void TrimExcess() {
        this.TrimExcess(this.Count);
      }

      // → Sets the capacity of this dictionary to hold up `capacity` entries without any further expansion of its backing storage
      public void TrimExcess(int capacity) {
        if (capacity < this.Count) throw new System.ArgumentOutOfRangeException("Lorem ipsum");

        int      count   = this.count;
        int      size    = Hash.GetPrime(capacity);
        Entry[]? entries = this.entries;

        // …
        if (size >= entries?.Length ?? 0)
        return;

        ++this.version;
        this.Initialize (size);
        this.CopyEntries(entries, count);
      }

      public bool TryAdd(TKey key, TValue value) {
        return this.TryInsert(key, value, InsertionBehavior.None);
      }

      // → Gets an instance of a type that may be used to perform operations on the current `PatchOdyssey.Dictionary<TKey, TValue>` using a `TAlternateKey` as a key instead of a `TKey`
      public bool TryGetAlternateLookup<TAlternateKey>(out AlternateLookup<TAlternateKey> lookup) where TAlternateKey : notnull /* allows ref struct */ {
        if (AlternateLookup<TAlternateKey>.IsCompatibleKey(this)) {
          lookup = new(this);
          return true;
        }

        lookup = default;
        return false;
      }

      public bool TryGetValue(TKey key, /* [System.Diagnostics.CodeAnalysis.MaybeNullWhen(false)] */ out TValue value) {
        ref TValue reference = ref FindValue(key);

        // …
        if (!System.Runtime.CompilerServices.Unsafe.IsNullRef(ref reference)) {
          value = reference;
          return true;
        }

        value = default;

        return false;
      }

      private bool TryInsert(TKey key, TValue value, InsertionBehavior behavior) {
        if (null == key) throw new System.ArgumentNullException("Lorem ipsum");
        if (null == this.buckets) this.Initialize(0);

        uint    hashCode       = (uint) (typeof(TKey).IsValueType && null == this.comparer ? key.GetHashCode() : this.comparer!.GetHashCode(key));
        ref int bucket         = ref this.GetBucket(hashCode);
        int     index          = bucket - 1; // → Value in `this.buckets` is one-based
        uint    collisionCount = 0;

        // …
        if (typeof(TKey).IsValueType && null == this.comparer) {
          while ((uint) index < (uint) this.entries.Length) {
            if (hashCode == this.entries[index].hashCode && System.Collections.Generic.EqualityComparer<TKey>.Default.Equals(this.entries[index].key, key)) {
              switch (behavior) {
                case InsertionBehavior.OverwriteExisting: this.entries[index].value = value; return true;
                case InsertionBehavior.ThrowOnExisting: throw new System.Exception("Lorem ipsum");
              }

              return false;
            }

            index = this.entries[index].next;

            if (++collisionCount > (uint)this.entries.Length)
            throw new System.InvalidOperationException("Lorem ipsum");
          }
        }

        else
          while ((uint) index < (uint) this.entries.Length) {
            if (this.entries[index].hashCode == hashCode && this.comparer.Equals(this.entries[index].key, key)) {
              switch (behavior) {
                case InsertionBehavior.OverwriteExisting: this.entries[index].value = value; return true;
                case InsertionBehavior.ThrowOnExisting: throw new System.Exception("Lorem ipsum");
              }

              return false;
            }

            index = this.entries[index].next;

            if (++collisionCount > (uint) this.entries.Length)
            throw new System.InvalidOperationException("Lorem ipsum");
          }

        // …
        if (this.freeCount > 0) {
          index           = this.freeList;
          this.freeCount -= 1;
          this.freeList   = Dictionary<TKey, TValue>.StartOfFreeList - this.entries[this.freeList].next;
        }

        else {
          int count = this.count;

          // …
          if (count == this.entries.Length) {
            this.Resize();
            bucket = ref this.GetBucket(hashCode);
          }

          index      = count;
          this.count = count + 1;
        }

        // …
        ref Entry entry = ref this.entries![index];

        this.version  += 1;
        entry.value    = value;
        entry.next     = bucket - 1; // → Value in `this.buckets` is one-based
        entry.key      = key;
        entry.hashCode = hashCode;
        bucket         = index + 1; // → Value in `this.buckets` is one-based

        if (!typeof(TKey).IsValueType && Hash.HashCollisionThreshold < collisionCount && this.comparer is NonRandomizedStringEqualityComparer)
        this.Resize(this.entries.Length, true); // → Value types never rehash

        return true;
      }

      void System.Collections.ICollection.CopyTo(System.Array array, int index) {
        if (null == array)                      throw new System.ArgumentNullException   ("Lorem ipsum");
        if (array.Rank != 1)                    throw new System.ArgumentException       ("Lorem ipsum");
        if (array.GetLowerBound(0) != 0)        throw new System.ArgumentException       ("Lorem ipsum");
        if ((uint) array.Length < (uint) index) throw new System.IndexOutOfRangeException("Lorem ipsum");
        if (this.Count > array.Length - index)  throw new System.ArgumentException       ("Lorem ipsum");

        // …
        if (array is KeyValuePair<TKey, TValue>[] pairs)
          this.CopyTo(pairs, index);

        else if (array is System.Collections.DictionaryEntry[] entries) {
          for (int subindex = 0; subindex != this.count; ++subindex) {
            if (this.entries![subindex].next >= -1)
            entries[index++] = new(this.entries[subindex].key, this.entries[subindex].value);
          }
        }

        else {
          object[]? objects = array as object[];

          // …
          if (null == objects)
          throw new System.ArgumentException("Lorem ipsum");

          try {
            for (int subindex = 0; subindex != this.count; ++subindex) {
              if (this.entries![subindex].next >= -1)
              objects[index++] = new System.Collections.Generic.KeyValuePair<TKey, TValue>(this.entries[subindex].key, this.entries[subindex].value);
            }
          } catch (System.ArrayTypeMismatchException) { throw new System.ArgumentException("Lorem ipsum"); }
        }
      }

      void System.Collections.IDictionary.Add(object key, object? value) {
          if (null == key /* → `|| null == value` */)
          throw new System.ArgumentNullException("Lorem ipsum");

          try {
            try { this.Add((TKey) key, (TValue) value!); }
            catch (System.InvalidCastException) { throw new System.ArgumentException("Lorem ipsum"); }
          } catch (System.InvalidCastException) { throw new System.ArgumentException("Lorem ipsum"); }
        }

        bool System.Collections.IDictionary.Contains(object key) {
          return this.IsCompatibleKey(key) ? this.ContainsKey((TKey) key) : false;
        }

        void System.Collections.IDictionary.Remove(object key) {
          if (this.IsCompatibleKey(key))
          this.Remove((TKey) key);
        }

      System.Collections.IDictionaryEnumerator System.Collections.IDictionary.GetEnumerator() {
        return new Enumerator(this, Enumerator.DictionaryEntry);
      }

      System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator() {
        return ((System.Collections.IEnumerable<System.Collections.Generic.KeyValuePair<TKey, TValue>>) this).GetEnumerator();
      }

      void System.Collections.Generic.ICollection<System.Collections.Generic.KeyValuePair<TKey, TValue>>.Add(System.Collections.Generic.KeyValuePair<TKey, TValue> pair) {
        this.Add(pair.Key, pair.Value);
      }

      bool System.Collections.Generic.ICollection<System.Collections.Generic.KeyValuePair<TKey, TValue>>.Contains(System.Collections.Generic.KeyValuePair<TKey, TValue> pair) {
        ref TValue value = ref this.FindValue(pair.Key);
        return !System.Runtime.CompilerServices.Unsafe.IsNullRef(ref value) && System.Collections.Generic.EqualityComparer<TValue>.Default.Equals(value, pair.Value);
      }

      void System.Collections.Generic.ICollection<System.Collections.Generic.KeyValuePair<TKey, TValue>>.CopyTo(System.Collections.Generic.KeyValuePair<TKey, TValue>[] array, int index) {
        this.CopyTo(array, index);
      }

      bool System.Collections.Generic.ICollection<System.Collections.Generic.KeyValuePair<TKey, TValue>>.Remove(System.Collections.Generic.KeyValuePair<TKey, TValue> pair) {
        ref TValue value = ref this.FindValue(pair.Key);

        // …
        if (!System.Runtime.CompilerServices.Unsafe.IsNullRef(ref value) && System.Collections.Generic.EqualityComparer<TValue>.Default.Equals(value, pair.Value)) {
          this.Remove(pair.Key);
          return true;
        }

        return false;
      }

      System.Collections.Generic.IEnumerator<System.Collections.Generic.KeyValuePair<TKey, TValue>> System.Collections.Generic.IEnumerable<System.Collections.Generic.KeyValuePair<TKey, TValue>>.GetEnumerator() {
        return 0 == Count ? GenericEmptyEnumerator<System.Collections.Generic.KeyValuePair<TKey, TValue>>.Instance : this.GetEnumerator();
      }

      public TValue this[TKey key] {
        get {
          ref TValue value = ref this.FindValue(key);

          // …
          if (!System.Runtime.CompilerServices.Unsafe.IsNullRef(ref value))
          return value;

          throw new System.Collections.Generic.KeyNotFoundException("Lorem ipsum");
          return default;
        }

        set { this.TryInsert(key, value, InsertionBehavior.OverwriteExisting); }
      }

      object? System.Collections.IDictionary.this[object key] {
        get {
          if (this.IsCompatibleKey(key)) {
            ref TValue value = ref this.FindValue((TKey) key);

            // …
            if (!System.Runtime.CompilerServices.Unsafe.IsNullRef(ref value))
            return value;
          }

          return null;
        }

        set {
          if (null == key || null == value)
          throw new System.ArgumentNullException("Lorem ipsum");

          try {
            try { this[(TKey) key] = (TValue) value!; }
            catch (System.InvalidCastException) { throw new System.ArgumentException("Lorem ipsum"); }
          } catch (System.InvalidCastException) { throw new System.ArgumentException("Lorem ipsum"); }
        }
      }

    }
  #endif
}

public class Program {
  /* Main --> del ….exe & cls && csc /out:….exe /t:exe ….cs && ….exe & del ….exe */
  static void Main(string[] arguments) {}
}
