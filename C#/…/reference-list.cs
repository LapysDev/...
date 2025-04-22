// CONSIDER a `PatchDictionary` (and `PatchReadOnlyDictionary`) instead of `EditorDictionary`
// ENSURE `EditorDictionary` `ref`s its internal `System.Array` fields
namespace System {
  [System.Diagnostics.CodeAnalysis.ExcludeFromCodeCoverage]
  [System.Diagnostics.DebuggerNonUserCode]
  public readonly struct Index : System.IEquatable<Index> {
    public  static   System.Index Start     => new(0);
    public  static   System.Index End       => new(~0);
    public           bool         IsFromEnd => this.value < 0;
    public           int          Value     => this.value < 0 ? ~this.value : this.value;
    private readonly int          value;

    /* … */
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] private Index(int value)                       { this.value = value; }
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public  Index(int value, bool fromEnd = false) { if (value < 0) { throw new System.ArgumentOutOfRangeException(nameof(value)); } this.value = fromEnd ? ~value : value; }

    /* … */
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public  override bool         Equals                         (object? value)  => value is Index index && this.Equals(index);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public           bool         Equals                         (Index   index)  => index.value == this.value;
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public  static   System.Index FromEnd                        (int     value)  { if (value < 0) { throw new System.IndexOutOfRangeException(nameof(value)); } return new(~value); }
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public  static   System.Index FromStart                      (int     value)  { if (value < 0) { throw new System.IndexOutOfRangeException(nameof(value)); } return new(value); }
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public  override int          GetHashCode                    ()               => this.value;
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public           int          GetOffset                      (int length)     => this.IsFromEnd ? this.value + (length + 1) : this.value;
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public  override string       ToString                       ()               => this.IsFromEnd ? this.ToStringFromEnd() : ((uint) this.Value).ToString();
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] private          string       ToStringFromEnd                ()               => $"^{this.Value.ToString()}";
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] bool                          System.IEquatable<Index>.Equals(Index index)    => this.Equals(index);

    /* … */
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public static implicit operator System.Index(int value) => Index.FromStart(value);
  }

  [System.Diagnostics.CodeAnalysis.ExcludeFromCodeCoverage]
  [System.Diagnostics.DebuggerNonUserCode]
  public readonly struct Range : System.IEquatable<Range> {
    public static /* readonly */ System.Range All => new(System.Index.Start, System.Index.End);

    public readonly System.Index End   { get; } = System.Index.End;
    public readonly System.Index Start { get; } = System.Index.Start;

    /* … */
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)]
    public Range(System.Index start, System.Index end) {
      this.End   = end;
      this.Start = start;
    }

    /* … */
    public static Range EndAt(System.Index end) {
      return new(System.Index.Start, end);
    }

    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public override bool Equals     (object?  value) => value is Range range && this.Equals(range);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public          bool Equals     (in Range range) => this.End.Equals(range.End) && this.Start.Equals(range.Start);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public override int  GetHashCode()               => this.End.GetHashCode() + this.Start.GetHashCode(); // TODO: HashCode.Combine

    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)]
    public (int Offset, int Length) GetOffsetAndLength(int length) {
      (System.Index endIndex, System.Index startIndex) = (this.End, this.Start);
      (int          end,      int          start)      = (endIndex.IsFromEnd ? length - endIndex.Value : endIndex.Value, startIndex.IsFromEnd ? length - startIndex.Value : startIndex.Value);

      // …
      if ((uint) end > (uint) length || (uint) end < (uint) start)
      throw new System.ArgumentOutOfRangeException(nameof(length));

      return (start, end - start);
    }

    public static Range StartAt(System.Index start) {
      return new(System.Index.End, start);
    }

    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public override string ToString                       ()            => $"{this.Start}..{this.End}";
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] bool                   System.IEquatable<Range>.Equals(Range range) => this.Equals(range);
  }
}

namespace System.Runtime.CompilerServices {
  public static partial class RuntimeHelpers {
    public static T[] GetSubArray<T>(T[] array, System.Range range) {
      (int offset, int length) = range.GetOffsetAndLength(array.Length);
      T[] subarray;

      // …
      if (typeof(T[]) == array.GetType()) {
        if (0 == length)
        return System.Array.Empty<T>();

        subarray = new T[length];
      } else subarray = System.Runtime.CompilerServices.Unsafe.As<T[]>(System.Array.CreateInstance(array.GetType(), length));

      System.Array.Copy(array, offset, subarray, 0, length);
      return subarray;
    }
  }
}

public static partial class Util {
  public static uint EnumerableCount<T>(System.Collections.Generic.IEnumerable<T> enumerable) {
    uint count = 0u;

    foreach (T _ in enumerable)
    ++count;

    return count;
  }
}

public static class Program {
  public delegate void RefAction            <T>   (ref T value);
  public delegate int  RefComparison        <T>   (ref T a, ref T b);
  public delegate U    RefConverter         <T, U>(ref T value);
  public delegate bool RefPredicate         <T>   (ref T value);
  public delegate void RefReadOnlyAction    <T>   (in  T value);
  public delegate int  RefReadOnlyComparison<T>   (in  T a, in  T b);
  public delegate U    RefReadOnlyConverter <T, U>(in  T value);
  public delegate bool RefReadOnlyPredicate <T>   (in  T value);

  public abstract class RefComparer        <T> : Program.RefReadOnlyComparer        <T>;
  public abstract class RefReadOnlyComparer<T> : System.Collections.Generic.Comparer<T>;

  public class RefReadOnlyList<T> : System.Collections.Generic.IEnumerable<T>, System.Collections.Generic.IReadOnlyList<T> /* ⟶ Based on `System.Collections.Generic.List<T>` and `System.Collections.ObjectModel.ReadOnlyCollection<T>` */ {
    public struct Enumerator : System.Collections.Generic.IEnumerator<T> {
      public   ref readonly T                  Current => ref this.list.GetValue((uint) this.index);
      internal              int                index;
      internal              RefReadOnlyList<T> list;
      T                                        System.Collections.Generic.IEnumerator<T>.Current => this.Current;
      object                                   System.Collections.IEnumerator.Current            => this.Current!;

      /* … */
      [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)]
      public Enumerator(RefReadOnlyList<T> list) {
        this.index = -1;
        this.list  = list;
      }

      /* … */
      [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public void Dispose                                () { /* Do nothing… */ }
      [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public bool MoveNext                               () => this.index < this.list.Count - 1 ? (++this.index, _: true)._ : false;
      [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public void Reset                                  () => this.index = -1;
      [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] bool        System.Collections.IEnumerator.MoveNext() => this.MoveNext();
      [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] void        System.Collections.IEnumerator.Reset   () => this.Reset   ();
      [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] void        System.IDisposable.Dispose             () => this.Dispose ();
    }

    /* … */
    public   uint Capacity                                                => this.Count;
    public   uint Count { get; protected set; }                           =  0u;
    internal T[]  Items                                                   =  System.Array.Empty<T>(); // ⟶ Faster with `System.GC.AllocateUninitializedArray<T>(…, false)`
    int           System.Collections.Generic.IReadOnlyCollection<T>.Count => ((int) this.Count);

    /* … ⟶ Availability of `System.Runtime.InteropServices.CollectionMarshal.AsSpan(…)` would replace `RefReadOnlyList<T>`’s entire purpose */
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public             RefReadOnlyList()                                                  {}
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] protected          RefReadOnlyList(uint               capacity)                       =>                                  this.Items = RefReadOnlyList<T>.CreateInstance(capacity);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public             RefReadOnlyList(RefReadOnlyList<T> list)                           { this.Count = list.Count;          this.Items = (T[]) list.Items.Clone(); }
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] protected internal RefReadOnlyList(T[]                array)                          { this.Count = (uint) array.Length; this.Items = array; }
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] protected          RefReadOnlyList(T[]                array, uint index, uint length) => System.Array.Copy(array, (int) index, this.Items = RefReadOnlyList<T>.CreateInstance(this.Count = length), 0, (int) length);

    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)]
    public RefReadOnlyList(System.Collections.Generic.IEnumerable<T> enumerable) {
      this.Count = Util.EnumerableCount(enumerable);

      if (0u != this.Count) {
        this.Items = new T[this.Count];

        using (System.Collections.Generic.IEnumerator<T> enumerator = enumerable.GetEnumerator()) {
          for (uint index = 0u; enumerator.MoveNext(); ++index)
          this.SetValue(enumerator.Current, index);
        }
      }
    }

    /* … */
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public            RefReadOnlyList<T>            AsCopy        ()                                                                                                                                                             => new(this);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public            RefReadOnlyList<T>            AsReadOnly    ()                                                                                                                                                             =>     this;
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public            int                           BinarySearch  (in T                               element)                                                                                                                   => System.Array.BinarySearch(this.Items, 0, (int) this.Count, element);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public            int                           BinarySearch  (in T                               element, System.Collections.Generic.IComparer<T>? comparer)                                                                => this.BinarySearch(0u, this.Count, element, comparer);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public            int                           BinarySearch  (uint                               index,   uint                                     length, in T element, System.Collections.Generic.IComparer<T>? comparer) => System.Array.BinarySearch(this.Items, (int) index, (int) length, element, comparer);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public            bool                          Contains      (in T                               element)                                                                                                                   => this.IndexOf(element, 0u, this.Count) != -1;
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public            RefReadOnlyList<U>            ConvertAll<U> (Program.RefReadOnlyConverter<T, U> converter)                                                                                                                 { RefReadOnlyList<U> list = new(this.Count); for (; list.Count != this.Count; ++list.Count) { list.SetValue(converter(in this.GetValue(list.Count)), list.Count); } return list; }
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public            RefReadOnlyList<U>            ConvertAll<U> (System.Converter            <T, U> converter)                                                                                                                 => this.ConvertAll((in T element) => converter(element));
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public            void                          CopyTo        (T[]                                array)                                                                                                                     => this.CopyTo(0u, array, 0u,    this.Count);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public            void                          CopyTo        (T[]                                array, uint index)                                                                                                         => this.CopyTo(0u, array, index, this.Count);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public            void                          CopyTo        (uint                               index, T[]  array, uint arrayIndex, uint length)                                                                           => System.Array.Copy(this.Items, index, array, arrayIndex, length); // ⟶ Possible over-read
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] internal static   T[]                           CreateInstance(uint                               length)                                                                                                                    => 0u != length ? new T[length] : System.Array.Empty<T>();
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public            bool                          Exists        (Program.RefReadOnlyPredicate<T>    predicate)                                                                                                                 { for (uint index = 0u; index != this.Count; ++index) { if (predicate(in this.GetValue(index))) return true; } return false; }
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public            bool                          Exists        (System.Predicate            <T>    predicate)                                                                                                                 => this.Exists((in T element) => predicate(element));
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public            T?                            Find          (Program.RefReadOnlyPredicate<T>    predicate)                                                                                                                 { for (uint index = 0u; index != this.Count; ++index) { ref readonly T element = ref this.GetValue(index); if (predicate(in element)) return element; } return default; }
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public            T?                            Find          (System.Predicate            <T>    predicate)                                                                                                                 => this.Find((in T element) => predicate(element));
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public            RefReadOnlyList<T>            FindAll       (Program.RefReadOnlyPredicate<T>    predicate)                                                                                                                 { RefReadOnlyList<T> list = new(this.Count); for (uint index = 0u; index != this.Count; ++index) { ref readonly T element = ref this.GetValue(index); if (predicate(in element)) list.SetValue(in element, list.Count++); } return list; }
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public            RefReadOnlyList<T>            FindAll       (System.Predicate            <T>    predicate)                                                                                                                 => this.FindAll  ((in T element) => predicate(element));
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public            int                           FindIndex     (Program.RefReadOnlyPredicate<T>    predicate)                                                                                                                 => this.FindIndex(0u, this.Count, predicate);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public            int                           FindIndex     (System.Predicate            <T>    predicate)                                                                                                                 => this.FindIndex(0u, this.Count, predicate);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public            int                           FindIndex     (uint                               index, Program.RefReadOnlyPredicate<T> predicate)                                                                          { for (; index < this.Count; ++index) { if (predicate(in this.GetValue(index))) return (int) index; } return -1; }
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public            int                           FindIndex     (uint                               index, System.Predicate            <T> predicate)                                                                          => this.FindIndex(index, index <= this.Count ? this.Count - index : 0u, predicate);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public            int                           FindIndex     (uint                               index, uint                            length, Program.RefReadOnlyPredicate<T> predicate)                                  { for (uint end = System.Math.Min(this.Count, index + length); end > index; ++index) { if (predicate(in this.GetValue(index))) return (int) index; } return -1; }
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public            int                           FindIndex     (uint                               index, uint                            length, System.Predicate            <T> predicate)                                  => System.Array.FindIndex(this.Items, (int) index, (int) length, predicate);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public            int                           FindLastIndex (Program.RefReadOnlyPredicate<T>    predicate)                                                                                                                 => this.FindLastIndex(0u, this.Count, predicate);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public            int                           FindLastIndex (System.Predicate            <T>    predicate)                                                                                                                 => this.FindLastIndex(0u, this.Count, predicate);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public            int                           FindLastIndex (uint                               index, Program.RefReadOnlyPredicate<T> predicate)                                                                          { for (uint end = this.Count; end-- > index; ) { if (predicate(in this.GetValue(end))) return (int) end; } return -1; }
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public            int                           FindLastIndex (uint                               index, System.Predicate            <T> predicate)                                                                          => this.FindLastIndex(index, index <= this.Count ? this.Count - index : 0u, predicate);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public            int                           FindLastIndex (uint                               index, uint                           length, Program.RefReadOnlyPredicate<T> predicate)                                   { for (uint end = System.Math.Min(this.Count, index + length); end-- > index; ) { if (predicate(in this.GetValue(end))) return (int) end; } return -1; }
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public            int                           FindLastIndex (uint                               index, uint                           length, System.Predicate            <T> predicate)                                   => System.Array.FindLastIndex(this.Items, (int) index, (int) length, predicate);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public            void                          ForEach       (Program.RefReadOnlyAction<T>       action)                                                                                                                    { for (uint index = 0u; index != this.Count; ++index) action(in this.GetValue(index)); }
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public            void                          ForEach       (System.Action            <T>       action)                                                                                                                    => this.ForEach((in T element) => action(element));
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public            RefReadOnlyList<T>.Enumerator GetEnumerator ()                                                                                                                                                             => new(this);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public            RefReadOnlyList<T>            GetRange      (uint index, uint length)                                                                                                                                      => new(this.Items, index, length);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] internal          ref T                         GetValue      (uint index)                                                                                                                                                   => ref System.Runtime.CompilerServices.Unsafe.Add(ref System.Runtime.InteropServices.MemoryMarshal.GetReference(this.Items), (int) index);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public            int                           IndexOf       (in T element)                                                                                                                                                 => this.IndexOf(element, 0u,    this.Count);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public            int                           IndexOf       (in T element, uint index)                                                                                                                                     => this.IndexOf(element, index, index <= this.Count ? this.Count - index : 0u);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public            int                           IndexOf       (in T element, uint index, uint length)                                                                                                                        => System.Array.IndexOf(this.Items, element, (int) index, (int) length);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public            int                           LastIndexOf   (in T element)                                                                                                                                                 => this.LastIndexOf(element, 0u,    this.Count);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public            int                           LastIndexOf   (in T element, uint index)                                                                                                                                     => this.LastIndexOf(element, index, index <= this.Count ? this.Count - index : 0u);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public            int                           LastIndexOf   (in T element, uint index, uint length)                                                                                                                        => System.Array.LastIndexOf(this.Items, element, (int) index, (int) length);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] internal          void                          SetValue      (in T element, uint index)                                                                                                                                     { ref T reference = ref System.Runtime.CompilerServices.Unsafe.Add(ref System.Runtime.InteropServices.MemoryMarshal.GetReference(this.Items), (int) index); reference = element; }
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public            RefReadOnlyList<T>            Slice         (uint index,   uint length)                                                                                                                                    => new(this.Items, index, length);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public            ref readonly T[]              ToArray       ()                                                                                                                                                             => ref this.Items;
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public   override string                        ToString      ()                                                                                                                                                             { uint end = this.Count, index = 0u; if (end != index) unsafe { System.Text.StringBuilder builder = new(); for (char* separator = stackalloc char[] {',', ' '}; ; builder.Append(separator, 2)) { builder.Append(this.GetValue(index)); if (end == ++index) return builder.ToString(); } } return string.Empty; }
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public            bool                          TrueForAll    (Program.RefReadOnlyPredicate<T> predicate)                                                                                                                    { for (uint index = 0u; index != this.Count; ++index) { if (!predicate(in this.GetValue(index))) return false; } return true; }
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public            bool                          TrueForAll    (System.Predicate            <T> predicate)                                                                                                                    => this.TrueForAll((in T element) => predicate(element));

    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] System.Collections.IEnumerator            System.Collections.IEnumerable.GetEnumerator           () => (System.Collections.IEnumerator)            this.GetEnumerator();
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] System.Collections.Generic.IEnumerator<T> System.Collections.Generic.IEnumerable<T>.GetEnumerator() => (System.Collections.Generic.IEnumerator<T>) this.GetEnumerator();

    /* … */
    public ref readonly T                  this                                            [uint         index] => ref this.GetValue(index);
    public              RefReadOnlyList<T> this                                            [System.Range range] => new(this.Items[range]);
    T                                      System.Collections.Generic.IReadOnlyList<T>.this[int          index] => this[(uint) index];
  }

  public class RefList<T> : Program.RefReadOnlyList<T>, System.Collections.Generic.IList<T>, System.Collections.ICollection, System.Collections.IList {
    public new struct Enumerator : System.Collections.Generic.IEnumerator<T> {
      public  ref T                                     Current => ref this.enumerator.list.GetValue((uint) this.enumerator.index);
      private     Program.RefReadOnlyList<T>.Enumerator enumerator;
      T                                                 System.Collections.Generic.IEnumerator<T>.Current => this.Current;
      object                                            System.Collections.IEnumerator.Current            => this.Current!;

      /* … */
      [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)]
      public Enumerator(RefList<T> list) {
        this.enumerator = new(list);
      }

      /* … */
      [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public void Dispose                                () => this.enumerator.Dispose ();
      [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public bool MoveNext                               () => this.enumerator.MoveNext();
      [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public void Reset                                  () => this.enumerator.Reset   ();
      [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] bool        System.Collections.IEnumerator.MoveNext() => this           .MoveNext();
      [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] void        System.Collections.IEnumerator.Reset   () => this           .Reset   ();
      [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] void        System.IDisposable.Dispose             () => this           .Dispose ();
    }

    /* … */
    public  new uint Capacity                                             { get => this.capacity; set => this.EnsureCapacity(value); }
    private     uint capacity                                             =  0u;
    int              System.Collections.Generic.ICollection<T>.Count      => ((int) this.Count);
    bool             System.Collections.Generic.ICollection<T>.IsReadOnly => false;
    int              System.Collections.ICollection.Count                 => ((int) this.Count);
    bool             System.Collections.ICollection.IsSynchronized        => false;
    object           System.Collections.ICollection.SyncRoot              => this;
    bool             System.Collections.IList.IsFixedSize                 => false;
    bool             System.Collections.IList.IsReadOnly                  => false;

    /* … */
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public    RefList()                                    : base()                                            {}
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public    RefList(uint                       capacity) : base(capacity = RefList<T>.GetCapacity(capacity)) => this.capacity = capacity;
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public    RefList(RefList<T>                 list)     : this((Program.RefReadOnlyList<T>) list)           {}
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public    RefList(Program.RefReadOnlyList<T> list)     : this(list.Items, 0u, list.Count)                  {}
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] protected RefList(T[]                        array, uint index, uint length)                               => System.Array.Copy(array, (int) index, this.Items = Program.RefReadOnlyList<T>.CreateInstance(this.capacity = RefList<T>.GetCapacity(this.Count = length)), 0, (int) length);

    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)]
    public RefList(System.Collections.Generic.IEnumerable<T> enumerable) {
      this.Count = Util.EnumerableCount(enumerable);

      if (0u != this.Count) {
        this.capacity = RefList<T>.GetCapacity(this.Count);
        this.Items    = new T[this.capacity];

        using (System.Collections.Generic.IEnumerator<T> enumerator = enumerable.GetEnumerator()) {
          for (uint index = 0u; enumerator.MoveNext(); ++index)
          this.SetValue(enumerator.Current, index);
        }
      }
    }

    /* … */
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public                 void                  Add           (in T                                      element)                                                   => this.Insert     (this.Count, element);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public    ref readonly T                     Append        (in T                                      element)                                                   {  this.Insert     (this.Count, element); return ref element; }
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public                 void                  AddRange      (System.Collections.Generic.IEnumerable<T> enumerable)                                                => this.InsertRange(this.Count, enumerable);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public    new          RefList<T>            AsCopy        ()                                                                                                    => new(this);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public                 void                  Clear         ()                                                                                                    { this.capacity = this.Count = 0u; this.Items = System.Array.Empty<T>(); }
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public                 RefList<U>            ConvertAll<U> (Program.RefConverter<T, U> converter)                                                                { RefList<U> list = new(this.Count); for (; list.Count != this.Count; ++list.Count) { list.SetValue(converter(ref this.GetValue(list.Count)), list.Count); } return list; }
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public                 uint                  EnsureCapacity(uint                       capacity)                                                                 { if (capacity > this.capacity) { T[] list = this.Items; System.Array.Copy(list, 0, this.Items = RefReadOnlyList<T>.CreateInstance(this.capacity = RefList<T>.GetCapacity(capacity)), 0, (int) this.Count); } return this.capacity; }
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public                 bool                  Exists        (Program.RefPredicate<T>    predicate)                                                                { for (uint index = 0u; index != this.Count; ++index) {                                           if (predicate(ref this.GetValue(index))) return true; }    return false; }
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public                 T?                    Find          (Program.RefPredicate<T>    predicate)                                                                { for (uint index = 0u; index != this.Count; ++index) { ref T element = ref this.GetValue(index); if (predicate(ref element))              return element; } return default; }
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public                 RefList<T>            FindAll       (Program.RefPredicate<T>    predicate)                                                                { RefList<T> list = new(this.Count); for (uint index = 0u; index != this.Count; ++index) { ref T element = ref this.GetValue(index); if (predicate(ref element)) list.SetValue(in element, list.Count++); } list.TrimExcess(); return list; }
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public                 int                   FindIndex     (Program.RefPredicate<T>    predicate)                                                                => this.FindIndex(0u, this.Count, predicate);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public                 int                   FindIndex     (uint                       index, Program.RefPredicate<T> predicate)                                 { for                                                       (; index < this.Count; ++index) { if (predicate(ref this.GetValue(index))) return (int) index; } return -1; }
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public                 int                   FindIndex     (uint                       index, uint                    length, Program.RefPredicate<T> predicate) { for (uint end = System.Math.Min(this.Count, index + length); index < end;        ++index) { if (predicate(ref this.GetValue(index))) return (int) index; } return -1; }
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public                 int                   FindLastIndex (Program.RefPredicate<T>    predicate)                                                                => this.FindLastIndex(0u, this.Count, predicate);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public                 int                   FindLastIndex (uint                       index, Program.RefPredicate<T> predicate)                                 { for (uint end = this.Count;                                  end-- > index; ) { if (predicate(ref this.GetValue(end))) return (int) end; } return -1; }
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public                 int                   FindLastIndex (uint                       index, uint                    length, Program.RefPredicate<T> predicate) { for (uint end = System.Math.Min(this.Count, index + length); end-- > index; ) { if (predicate(ref this.GetValue(end))) return (int) end; } return -1; }
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public                 void                  ForEach       (Program.RefAction<T>       action)                                                                   { for (uint index = 0u; index != this.Count; ++index) action(ref this.GetValue(index)); }
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] protected static       uint                  GetCapacity   (uint                       count)                                                                    { if (0u != count) { count = System.Math.Max(count, 4u) - 1u; count |= count >> 1; count |= count >> 2; count |= count >> 4; count |= count >> 8; count |= count >> 16; return count + 1u; } return 0u; }
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public    new          RefList<T>.Enumerator GetEnumerator ()                                                                                                    => new(this);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public    new          RefList<T>            GetRange      (uint index, uint length)                                                                             => new(this.Items, index, length);

    private void HeapSort(uint begin, uint end, Program.RefComparison<T> comparison) /* ⟶ See `https://web.archive.org/web/20170509032649/http://www.cdn.geeksforgeeks.org/heap-sort/` */ {
      [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)]
      static void Build(RefList<T> list, uint end, uint index, Program.RefComparison<T> comparison) {
        uint largest = index;
        uint left    = (index * 2u) + 1u;
        uint right   = (index * 2u) + 2u;

        // …
        largest = end > left  && comparison(ref list.GetValue(largest), ref list.GetValue(left))  < 0 ? left  : largest;
        largest = end > right && comparison(ref list.GetValue(largest), ref list.GetValue(right)) < 0 ? right : largest;

        if (index != largest) {
          (list.GetValue(index), list.GetValue(largest)) = (list.GetValue(largest), list.GetValue(index));
          Build(list, end, largest, comparison);
        }
      }

      for (uint index = begin + ((end - begin) / 2u); begin != index--; )
      Build(this, end, index, comparison);

      for (uint index = end; begin != index--; ) {
        (this.GetValue(begin), this.GetValue(index)) = (this.GetValue(index), this.GetValue(begin));
        Build(this, index, begin, comparison);
      }
    }

    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)]
    public void Insert(uint index, in T element) {
      if (this.capacity == this.Count)
        this.EnsureCapacity(this.Count + 1u);

      if (index <= this.Count) {
        for (uint subindex = this.Count; index != subindex--; )
          this.SetValue(in this.GetValue(subindex + 0u), subindex + 1u);

        this.SetValue(in element, index);
      }

      ++this.Count;
    }

    private void InsertionSort(uint begin, uint end, Program.RefComparison<T> comparison) /* ⟶ See `https://web.archive.org/web/20240629152053/https://www.geeksforgeeks.org/insertion-sort-algorithm/` */ {
      for (uint index = begin + 1u; end != index; ++index) {
        T   element  = this.GetValue(index);
        int subindex = ((int) index) - 1;

        // …
        for (; subindex >= 0 && comparison(ref this.GetValue((uint) subindex), ref element) > 0; --subindex)
          this.SetValue(in this.GetValue((uint) subindex + 0u), (uint) subindex + 1u);

        this.SetValue(in element, (uint) ++subindex);
      }
    }

    public void InsertRange(uint index, System.Collections.Generic.IEnumerable<T> enumerable) {
      uint count = Util.EnumerableCount(enumerable);

      // …
      if (this.capacity < this.Count + count)
      this.EnsureCapacity(this.Count + count);

      if (index < this.Count) {
        for (uint subindex = this.Count - index; 0u != subindex--; )
          this.SetValue(in this.GetValue(index + subindex), count + index + subindex);

        this.Count += count;
      } else /* if (index > this.Count) */ (count, this.Count) = (count - System.Math.Min(count, index - this.Count), this.Count + count);

      using (System.Collections.Generic.IEnumerator<T> enumerator = enumerable.GetEnumerator())
      while (0u != count--) {
        enumerator.MoveNext();
        this.SetValue(enumerator.Current, index++);
      }
    }

    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)]
    public ref readonly T Prepend(in T element) {
      this.Insert(0, in element);
      return ref element;
    }

    private void QuickSort(uint begin, uint end, Program.RefComparison<T> comparison) /* ⟶ See `https://web.archive.org/web/20240629152053/https://www.geeksforgeeks.org/quick-sort-algorithm/` */ {
      [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)]
      static uint Partition(RefList<T> list, uint begin, uint end, Program.RefComparison<T> comparison) {
        int   index = ((int) begin) - 1;
        ref T pivot = ref list.GetValue(end);

        // …
        for (uint subindex = begin; end > subindex; ++subindex)
        if (comparison(ref list.GetValue(subindex), ref pivot) < 0) {
          ++index;
          (list.GetValue((uint) index), list.GetValue(subindex)) = (list.GetValue(subindex), list.GetValue((uint) index));
        }

        ++index;
        (list.GetValue((uint) index), list.GetValue(end)) = (list.GetValue(end), list.GetValue((uint) index));

        return (uint) index;
      }

      // …
      if (begin < end) {
        uint index = Partition(this, begin, end, comparison);

        this.QuickSort(begin,      index - 1u, comparison);
        this.QuickSort(index + 1u, end,        comparison);
      }
    }

    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public      bool       Remove                                            (in T                            element)                                                                          { int index = this.IndexOf(element); if (index != -1) { this.RemoveAt((uint) index); return true; } return false; }
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public      uint       RemoveAll                                         (Program.RefPredicate        <T> predicate)                                                                        { T[] array = (T[]) this.Items.Clone(); uint length = 0u; for (uint index = 0u; index != this.Count; ++index) { ref T element = ref this.GetValue(index); if (!predicate(ref element)) array[length++] = element; } length = this.Count - length; this.Count -= length; this.Items = array; return length; }
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public      uint       RemoveAll                                         (Program.RefReadOnlyPredicate<T> predicate)                                                                        => this.RemoveAll((ref T element) => predicate(in element));
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public      uint       RemoveAll                                         (System.Predicate            <T> predicate)                                                                        => this.RemoveAll((ref T element) => predicate   (element));
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public      void       RemoveAt                                          (uint                            index)                                                                            { while                                (++index < this.Count)                      { this.SetValue(in this.GetValue(index - 0u), index - 1u); } this.Count -= index == this.Count ? 1u : 0u; }
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public      void       RemoveRange                                       (uint                            index, uint length)                                                               { for (uint subindex = index + length; subindex < this.Count; ++index, ++subindex) { this.SetValue(in this.GetValue(subindex),   index); }      this.Count -= index <= this.Count ? this.Count - index : 0u; }
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public      void       Reverse                                           ()                                                                                                                 => this.Reverse        (0u, this.Count);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public      void       Reverse                                           (uint index, uint length)                                                                                          => System.Array.Reverse(this.Items, (int) index, (int) length);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public  new RefList<T> Slice                                             (uint index, uint length)                                                                                          => new(this.Items, index, length);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public      void       Sort                                              ()                                                                                                                 => this.Sort(0u, this.Count, System.Collections.Generic.Comparer<T>.Default);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public      void       Sort                                              (System.Collections.Generic.IComparer<T>? comparer)                                                                => this.Sort(0u, this.Count, comparer);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public      void       Sort                                              (Program.RefComparison               <T>  comparison)                                                              => this.Sort(0u, this.Count, comparison);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public      void       Sort                                              (Program.RefReadOnlyComparison       <T>  comparison)                                                              => this.Sort((ref T a, ref T b) => comparison(in a, in b));
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public      void       Sort                                              (System.Comparison                   <T>  comparison)                                                              => this.Sort((ref T a, ref T b) => comparison(a,    b));
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] private     void       Sort                                              (uint                                     begin, uint end,    Program.RefComparison               <T>  comparison) { if (this.Count <= 16u) this.InsertionSort(begin, end, comparison); else if (this.Count > System.Math.Log(this.Count) * 2.0) this.HeapSort(begin, end, comparison); else this.QuickSort(begin, end - 1u, comparison); }
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public      void       Sort                                              (uint                                     index, uint length, System.Collections.Generic.IComparer<T>? comparer)   { comparer ??= System.Collections.Generic.Comparer<T>.Default; this.Sort(index, System.Math.Min(this.Count, index + length), (ref T a, ref T b) => comparer!.Compare(a, b)); }
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public  new ref T[]    ToArray                                           ()                                                                                                                 => ref this.Items;
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public      void       TrimExcess                                        ()                                                                                                                 => this.TrimExcess(this.Count);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public      void       TrimExcess                                        (uint                    capacity)                                                                                 { capacity = RefList<T>.GetCapacity(capacity); if (capacity < this.capacity && capacity >= this.Count) System.Array.Resize(ref this.Items, (int) (this.capacity = capacity)); }
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] public      bool       TrueForAll                                        (Program.RefPredicate<T> predicate)                                                                                { for (uint index = 0u; index != this.Count; ++index) { if (!predicate(ref this.GetValue(index))) return false; } return true; }
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] void                   System.Collections.Generic.ICollection<T>.Add     (T                       element)                                                                                  => this      .Add     (element);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] void                   System.Collections.Generic.ICollection<T>.Clear   ()                                                                                                                 => this      .Clear   ();
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] bool                   System.Collections.Generic.ICollection<T>.Contains(T            element)                                                                                             => this      .Contains(element);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] void                   System.Collections.Generic.ICollection<T>.CopyTo  (T[]          array, int index)                                                                                    => this      .CopyTo  (array, (uint) index);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] bool                   System.Collections.Generic.ICollection<T>.Remove  (T            element)                                                                                             => this      .Remove  (element);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] int                    System.Collections.Generic.IList<T>.IndexOf       (T            element)                                                                                             => this      .IndexOf (element);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] void                   System.Collections.Generic.IList<T>.Insert        (int          index, T element)                                                                                    => this      .Insert  ((uint) index, element);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] void                   System.Collections.Generic.IList<T>.RemoveAt      (int          index)                                                                                               => this      .RemoveAt((uint) index);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] void                   System.Collections.ICollection.CopyTo             (System.Array array, int index)                                                                                    => this.Items.CopyTo  (array, index);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] int                    System.Collections.IList.Add                      (object?      element)                                                                                             {  this      .Add     ((T) element!); return (int) this.Count; }
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] void                   System.Collections.IList.Clear                    ()                                                                                                                 => this      .Clear   ();
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] bool                   System.Collections.IList.Contains                 (object? element)                                                                                                  => System.Array.IndexOf(this.Items,       element) != this.Items.GetLowerBound(0) - 1;
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] int                    System.Collections.IList.IndexOf                  (object? element)                                                                                                  => System.Array.IndexOf(this.Items,       element);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] void                   System.Collections.IList.Insert                   (int     index, object? element)                                                                                   => this.Insert  ((uint) index, (T) element!);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] void                   System.Collections.IList.Remove                   (object? element)                                                                                                  => this.Remove  ((T) element!);
    [System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.AggressiveInlining)] void                   System.Collections.IList.RemoveAt                 (int     index)                                                                                                    => this.RemoveAt((uint) index);

    /* … */
    public new ref T          this                                    [uint         index] => ref this.GetValue(index);
    public new     RefList<T> this                                    [System.Range range] => new(this.Items[range]);
    T                         System.Collections.Generic.IList<T>.this[int          index] { get => this[(uint) index]; set => this[(uint) index] = value; }
    object?                   System.Collections.IList.this           [int          index] { get => this[(uint) index]; set => this[(uint) index] = (T) value!; }
  }

  /* Main */
  static void Main(string[] arguments) {}
}
