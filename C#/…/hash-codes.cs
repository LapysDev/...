#if !(NETCOREAPP2_1_OR_GREATER || NETSTANDARD2_1_OR_GREATER)
  #pragma warning disable CA1066 // ⟶ Implement `System.IEquatable<T>` when overriding `System.Object.Equals(…)`; Equality comparison of `System.HashCode` is disabled by design
  #pragma warning disable CA1815 // ⟶ Override equals and operator equals on value types; Equality comparison of `System.HashCode` is disabled by design
  #pragma warning disable CA2231 // ⟶ Overload operator equals on overriding value type `Equals(…)`; Equality comparison of `System.HashCode` is disabled by design
  #pragma warning disable SA1202 // ⟶ Elements should be ordered by access; Preserving original layout of members

  namespace System {
    // ⟶ `System.HashCode` @ `https://web.archive.org/web/20250115200832/https://learn.microsoft.com/en-us/dotnet/api/system.hashcode?view=net-9.0`
    public struct HashCode /* ⟶ Uses non-cryptographic “xxHash32” (https://web.archive.org/web/20150814055024/https://github.com/Cyan4973/xxHash) hash algorithm */ {
      private const           uint Prime1 = 2654435761u;
      private const           uint Prime2 = 2246822519u;
      private const           uint Prime3 = 3266489917u;
      private const           uint Prime4 = 668265263u;
      private const           uint Prime5 = 374761393u;
      private static readonly uint Seed   = HashCode.GenerateGlobalSeed();

      private uint length;
      private uint queueA;
      private uint queueB;
      private uint queueC;
      private uint valueA;
      private uint valueB;
      private uint valueC;
      private uint valueD;

      /* … */
      private void Add(int value) {
        uint length   = this.length++;
        uint position = length % 4;

        // … ⟶ `switch` statements can not be inlined
        if (position == 0u) { this.queueA = (uint) value; return; }
        if (position == 1u) { this.queueB = (uint) value; return; }
        if (position == 2u) { this.queueC = (uint) value; return; }

        /* if (position == 3u) */ {
          if (length == 3u)
            HashCode.Initialize(out this.valueA, out this.valueB, out this.valueC, out this.valueD);

          this.valueA = HashCode.Round(this.valueA, this.queueA);
          this.valueB = HashCode.Round(this.valueB, this.queueB);
          this.valueC = HashCode.Round(this.valueC, this.queueC);
          this.valueD = HashCode.Round(this.valueD, (uint) value);
        }
      }

      [PatchMethod(AggressiveInlining)] public void Add<T>(in T value)                                                            => this.Add(value?.GetHashCode() ?? 0);
      [PatchMethod(AggressiveInlining)] public void Add<T>(in T value, System.Collections.Generic.IEqualityComparer<T>? comparer) => this.Add(value is not null ? comparer?.GetHashCode(value) ?? value.GetHashCode() : 0);

      public void AddBytes(in System.ReadOnlySpan<byte> value) {
        ref byte position = ref System.Runtime.InteropServices.MemoryMarshal.GetReference(value);
        ref byte end      = ref System.Runtime.CompilerServices.Unsafe.Add               (ref position, value.Length);

        // Add four bytes at a time until the input has fewer than four bytes remaining.
        while ((nint) System.Runtime.CompilerServices.Unsafe.ByteOffset(ref position, ref end) >= sizeof(int)) {
          this.Add(System.Runtime.CompilerServices.Unsafe.ReadUnaligned<int>(ref position));
          position = ref System.Runtime.CompilerServices.Unsafe.Add(ref position, sizeof(int));
        }

        // Add the remaining bytes a single byte at a time.
        while (System.Runtime.CompilerServices.Unsafe.IsAddressLessThan(ref position, ref end)) {
          this.Add((int) position);
          position = ref System.Runtime.CompilerServices.Unsafe.Add(ref position, 1);
        }
      }

      [PatchMethod(AggressiveInlining)]
      public static int Combine<T1>(in T1 objectA) {
        uint hash  = HashCode.MixEmptyState() + 4u;
        uint hashA = (uint) (objectA?.GetHashCode() ?? 0);

        // …
        hash = HashCode.QueueRound(hash, hashA);

        return (int) HashCode.MixFinal(hash);
      }

      [PatchMethod(AggressiveInlining)]
      public static int Combine<T1, T2>(in T1 objectA, in T2 objectB) {
        uint hash                = HashCode.MixEmptyState() + 8u;
        (uint hashA, uint hashB) = ((uint) (objectA?.GetHashCode() ?? 0), (uint) (objectB?.GetHashCode() ?? 0));

        // …
        hash = HashCode.QueueRound(hash, hashA);
        hash = HashCode.QueueRound(hash, hashB);

        return (int) HashCode.MixFinal(hash);
      }

      [PatchMethod(AggressiveInlining)]
      public static int Combine<T1, T2, T3>(in T1 objectA, in T2 objectB, in T3 objectC) {
        uint hash                            = HashCode.MixEmptyState() + 12u;
        (uint hashA, uint hashB, uint hashC) = ((uint) (objectA?.GetHashCode() ?? 0), (uint) (objectB?.GetHashCode() ?? 0), (uint) (objectC?.GetHashCode() ?? 0));

        // …
        hash = HashCode.QueueRound(hash, hashA);
        hash = HashCode.QueueRound(hash, hashB);
        hash = HashCode.QueueRound(hash, hashC);

        return (int) HashCode.MixFinal(hash);
      }

      [PatchMethod(AggressiveInlining)]
      public static int Combine<T1, T2, T3, T4>(in T1 objectA, in T2 objectB, in T3 objectC, in T4 objectD) {
        uint hash;
        (uint hashA, uint hashB, uint hashC, uint hashD) = ((uint) (objectA?.GetHashCode() ?? 0), (uint) (objectB?.GetHashCode() ?? 0), (uint) (objectC?.GetHashCode() ?? 0), (uint) (objectD?.GetHashCode() ?? 0));

        // …
        HashCode.Initialize(out uint valueA, out uint valueB, out uint valueC, out uint valueD);

        (valueA, valueB, valueC, valueD) = (HashCode.Round(valueA, hashA), HashCode.Round(valueB, hashB), HashCode.Round(valueC, hashC), HashCode.Round(valueD, hashD));
        hash                             = HashCode.MixState(valueA, valueB, valueC, valueD) + 16u;

        return (int) HashCode.MixFinal(hash);
      }

      [PatchMethod(AggressiveInlining)]
      public static int Combine<T1, T2, T3, T4, T5>(in T1 objectA, in T2 objectB, in T3 objectC, in T4 objectD, in T5 objectE) {
        uint hash;
        (uint hashA, uint hashB, uint hashC, uint hashD, uint hashE) = ((uint) (objectA?.GetHashCode() ?? 0), (uint) (objectB?.GetHashCode() ?? 0), (uint) (objectC?.GetHashCode() ?? 0), (uint) (objectD?.GetHashCode() ?? 0), (uint) (objectE?.GetHashCode() ?? 0));

        // …
        HashCode.Initialize(out uint valueA, out uint valueB, out uint valueC, out uint valueD);

        (valueA, valueB, valueC, valueD) = (HashCode.Round(valueA, hashA), HashCode.Round(valueB, hashB), HashCode.Round(valueC, hashC), HashCode.Round(valueD, hashD));
        hash                             = HashCode.MixState  (valueA, valueB, valueC, valueD) + 20u;
        hash                             = HashCode.QueueRound(hash, hashE);

        return (int) HashCode.MixFinal(hash);
      }

      [PatchMethod(AggressiveInlining)]
      public static int Combine<T1, T2, T3, T4, T5, T6>(in T1 objectA, in T2 objectB, in T3 objectC, in T4 objectD, in T5 objectE, in T6 objectF) {
        uint hash;
        (uint hashA, uint hashB, uint hashC, uint hashD, uint hashE, uint hashF) = ((uint) (objectA?.GetHashCode() ?? 0), (uint) (objectB?.GetHashCode() ?? 0), (uint) (objectC?.GetHashCode() ?? 0), (uint) (objectD?.GetHashCode() ?? 0), (uint) (objectE?.GetHashCode() ?? 0), (uint) (objectF?.GetHashCode() ?? 0));

        // …
        HashCode.Initialize(out uint valueA, out uint valueB, out uint valueC, out uint valueD);

        (valueA, valueB, valueC, valueD) = (HashCode.Round(valueA, hashA), HashCode.Round(valueB, hashB), HashCode.Round(valueC, hashC), HashCode.Round(valueD, hashD));
        hash                             = HashCode.MixState  (valueA, valueB, valueC, valueD) + 24u;
        hash                             = HashCode.QueueRound(hash, hashE);
        hash                             = HashCode.QueueRound(hash, hashF);

        return (int) HashCode.MixFinal(hash);
      }

      [PatchMethod(AggressiveInlining)]
      public static int Combine<T1, T2, T3, T4, T5, T6, T7>(in T1 objectA, in T2 objectB, in T3 objectC, in T4 objectD, in T5 objectE, in T6 objectF, in T7 objectG) {
        uint hash;
        (uint hashA, uint hashB, uint hashC, uint hashD, uint hashE, uint hashF, uint hashG) = ((uint) (objectA?.GetHashCode() ?? 0), (uint) (objectB?.GetHashCode() ?? 0), (uint) (objectC?.GetHashCode() ?? 0), (uint) (objectD?.GetHashCode() ?? 0), (uint) (objectE?.GetHashCode() ?? 0), (uint) (objectF?.GetHashCode() ?? 0), (uint) (objectG?.GetHashCode() ?? 0));

        // …
        HashCode.Initialize(out uint valueA, out uint valueB, out uint valueC, out uint valueD);

        (valueA, valueB, valueC, valueD) = (HashCode.Round(valueA, hashA), HashCode.Round(valueB, hashB), HashCode.Round(valueC, hashC), HashCode.Round(valueD, hashD));
        hash                             = HashCode.MixState  (valueA, valueB, valueC, valueD) + 28u;
        hash                             = HashCode.QueueRound(hash, hashE);
        hash                             = HashCode.QueueRound(hash, hashF);
        hash                             = HashCode.QueueRound(hash, hashG);

        return (int) HashCode.MixFinal(hash);
      }

      [PatchMethod(AggressiveInlining)]
      public static int Combine<T1, T2, T3, T4, T5, T6, T7, T8>(in T1 objectA, in T2 objectB, in T3 objectC, in T4 objectD, in T5 objectE, in T6 objectF, in T7 objectG, in T8 objectH) {
        uint hash;
        (uint hashA, uint hashB, uint hashC, uint hashD, uint hashE, uint hashF, uint hashG, uint hashH) = ((uint) (objectA?.GetHashCode() ?? 0), (uint) (objectB?.GetHashCode() ?? 0), (uint) (objectC?.GetHashCode() ?? 0), (uint) (objectD?.GetHashCode() ?? 0), (uint) (objectE?.GetHashCode() ?? 0), (uint) (objectF?.GetHashCode() ?? 0), (uint) (objectG?.GetHashCode() ?? 0), (uint) (objectH?.GetHashCode() ?? 0));

        // …
        HashCode.Initialize(out uint valueA, out uint valueB, out uint valueC, out uint valueD);

        (valueA, valueB, valueC, valueD) = (HashCode.Round(valueA, hashA), HashCode.Round(valueB, hashB), HashCode.Round(valueC, hashC), HashCode.Round(valueD, hashD));
        (valueA, valueB, valueC, valueD) = (HashCode.Round(valueA, hashE), HashCode.Round(valueB, hashF), HashCode.Round(valueC, hashG), HashCode.Round(valueD, hashH));
        hash                             = HashCode.MixState(valueA, valueB, valueC, valueD) + 32u;

        return (int) HashCode.MixFinal(hash);
      }

      [PatchMethod(AggressiveInlining)]
      private static void Initialize(out uint valueA, out uint valueB, out uint valueC, out uint valueD) {
        valueA = HashCode.Seed + HashCode.Prime1 + HashCode.Prime2;
        valueB = HashCode.Seed + HashCode.Prime2;
        valueC = HashCode.Seed;
        valueD = HashCode.Seed - HashCode.Prime1;
      }

      [PatchMethod(AggressiveInlining)] private static uint MixEmptyState()                                                   => HashCode.Prime5 + HashCode.Seed;
      [PatchMethod(AggressiveInlining)] private static uint MixFinal     (uint hash)                                          { hash ^= hash >> 15; hash *= HashCode.Prime2; hash ^= hash >> 13; hash *= HashCode.Prime3; hash ^= hash >> 16; return hash; }
      [PatchMethod(AggressiveInlining)] private static uint MixState     (uint valueA, uint valueB, uint valueC, uint valueD) => HashCode.RotateLeft(valueA, 1) + HashCode.RotateLeft(valueB, 7) + HashCode.RotateLeft(valueC, 12) + HashCode.RotateLeft(valueD, 18);
      [PatchMethod(AggressiveInlining)] private static uint QueueRound   (uint hash,   uint queuedValue)                      => HashCode.Prime4 * HashCode.RotateLeft(hash + (HashCode.Prime3 * queuedValue), 17);
      [PatchMethod(AggressiveInlining)] private static uint RotateLeft   (uint value,  int  offset)                           => (value << offset) | (value >> (32 - offset));
      [PatchMethod(AggressiveInlining)] private static uint Round        (uint hash,   uint input)                            => HashCode.Prime1 * HashCode.RotateLeft(hash + (HashCode.Prime2 * input),       13);

      public int ToHashCode() {
        uint hash     = this.length < 4u ? HashCode.MixEmptyState() : HashCode.MixState(this.valueA, this.valueB, this.valueC, this.valueD);
        uint position = this.length % 4u;

        // …
        hash += this.length * 4u;

        if (position > 0u) { hash = HashCode.QueueRound(hash, this.queueA);
        if (position > 1u) { hash = HashCode.QueueRound(hash, this.queueB);
        if (position > 2u) { hash = HashCode.QueueRound(hash, this.queueC);
        } } }

        return (int) HashCode.MixFinal(hash);
      }

      #pragma warning disable CS0809 // ⟶ Obsolete member overrides non-obsolete member; Disallowing `HashCode::Equals(…)` and `HashCode::GetHashCode()` is by design
      #pragma warning disable CA1065 // ⟶ Do not raise exceptions in unexpected locations — also by design
        [System.ComponentModel.EditorBrowsable(System.ComponentModel.EditorBrowsableState.Never)]
        [System.Obsolete("`HashCode` is a mutable `struct` and should not be compared with other `HashCode`s.", error: true)]
        public override bool Equals(object? _) => throw new System.NotSupportedException("`HashCode` is a mutable `struct` and should not be compared with other `HashCode`s.");

        [System.ComponentModel.EditorBrowsable(System.ComponentModel.EditorBrowsableState.Never)]
        [System.Obsolete("`HashCode` is a mutable `struct` and should not be compared with other `HashCode`s. Use `HashCode.ToHashCode()` to retrieve the computed hash code.", error: true)]
        public override int GetHashCode() => throw new System.NotSupportedException("`HashCode` is a mutable `struct` and should not be compared with other `HashCode`s. Use `HashCode.ToHashCode()` to retrieve the computed hash code.");
      #pragma warning restore CA1065
      #pragma warning restore CS0809

      #pragma warning disable CA5394
        // ⟶ The .NET runtime uses `Interop.GetRandomBytes(…)`, but this suffices for a polyfill
        // ⟶ Avoid insecure randomness; Use a different value at each program execution, no cryptography involved
        private static uint GenerateGlobalSeed() => (uint) new System.Random().Next(int.MinValue, int.MaxValue);
      #pragma warning restore CA5394
    }
  }
#endif
