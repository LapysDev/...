#nullable enable annotations

public class Program {
  public static void Loop(int begin, int end, System.Delegate callback, bool reversed = false) {
    System.Reflection.ParameterInfo[]? parameters = callback?.Method.GetParameters();
    System.Reflection.MethodInfo?      method     = callback?.GetType().GetMethod("Invoke");
    int                                direction  = System.Math.Sign(end - begin);
    object[]                           arguments  = new object[] {begin, System.Math.Abs(end - begin), begin, end};

    static System.Delegate DelegateConvert(System.Type typeA, System.Type typeB) {
      System.Linq.Expressions.ParameterExpression expression = System.Linq.Expressions.Expression.Parameter(typeA);
      return System.Linq.Expressions.Expression.Lambda(System.Linq.Expressions.Expression.Convert(expression, typeB), expression).Compile();
    }

    // …
    if (null == method || null == parameters) return;
    if (reversed) { (begin, end) = (end, begin); direction = -direction; }

    try { System.Array.Resize(ref arguments, parameters.Length); }
    catch (System.Exception) { throw new System.NotSupportedException("Cannot `Loop(…)` given specified callback; Too many parameters"); }

    for (int index = 0; index != parameters.Length; ++index)
    arguments[index] = DelegateConvert(arguments[index].GetType(), parameters[index].ParameterType).DynamicInvoke(arguments[index]);

    for ((int index, System.Delegate Convert) = (begin, 0 != parameters.Length ? DelegateConvert(typeof(int), parameters[0].ParameterType) : null); ; index += direction) {
      if (null != Convert)
        arguments[0] = Convert.DynamicInvoke(index);

      method.Invoke(callback, arguments);
      if (end == index) return;
    }
  }

  public static void Loop<T>(System.Collections.Generic.IEnumerable<T> enumerable, System.Delegate callback, bool reversed = false) {
    System.Reflection.ParameterInfo[]?      parameters = callback?.Method.GetParameters();
    System.Reflection.MethodInfo?           method     = callback?.GetType().GetMethod("Invoke");
    System.Collections.Generic.List<object> loopable   = new();
    int                                     index      = 0;
    object[]                                arguments  = new object[] {null, index};

    static System.Delegate DelegateConvert(System.Type typeA, System.Type typeB) {
      System.Linq.Expressions.ParameterExpression expression = System.Linq.Expressions.Expression.Parameter(typeA);
      return System.Linq.Expressions.Expression.Lambda(System.Linq.Expressions.Expression.Convert(expression, typeB), expression).Compile();
    }

    // …
    if (null == method || null == parameters) return;
    foreach (T value in enumerable) { loopable.Add(value); ++index; }
    if      (reversed)                loopable.Reverse();

    try { System.Array.Resize(ref arguments, parameters.Length); }
    catch (System.Exception) { throw new System.NotSupportedException("Cannot `Loop(…)` given specified callback; Too many parameters"); }

    loopable.Capacity = index;
    index             = reversed ? index - 1 : 0;

    foreach (object value in loopable) {
      if (parameters.Length > 0) arguments[0] = (T) value;
      if (parameters.Length > 1) arguments[1] = DelegateConvert(typeof(int), parameters[1].ParameterType).DynamicInvoke(index);

      index += reversed ? -1 : +1;
      method.Invoke(callback, arguments);
    }
  }

  public static void Loop   (int count,                                            System.Delegate                   callback, bool reversed = false) { if (0 != count) Loop(System.Math.Sign(count), count, callback, reversed); }
  public static void Loop   (int count,                                            System.Action<int>                callback, bool reversed = false) => Loop(count,      (System.Delegate) callback, reversed);
  public static void Loop   (int count,                                            System.Action<int, int>           callback, bool reversed = false) => Loop(count,      (System.Delegate) callback, reversed);
  public static void Loop   (int count,                                            System.Action<int, int, int, int> callback, bool reversed = false) => Loop(count,      (System.Delegate) callback, reversed);
  public static void Loop   (int begin, int end,                                   System.Action<int>                callback, bool reversed = false) => Loop(begin, end, (System.Delegate) callback, reversed);
  public static void Loop   (int begin, int end,                                   System.Action<int, int>           callback, bool reversed = false) => Loop(begin, end, (System.Delegate) callback, reversed);
  public static void Loop   (int begin, int end,                                   System.Action<int, int, int, int> callback, bool reversed = false) => Loop(begin, end, (System.Delegate) callback, reversed);
  public static void Loop<T>(System.Collections.Generic.IEnumerable<T> enumerable, System.Action<T>                  callback, bool reversed = false) => Loop(enumerable, (System.Delegate) callback, reversed);
  public static void Loop<T>(System.Collections.Generic.IEnumerable<T> enumerable, System.Action<T, int>             callback, bool reversed = false) => Loop(enumerable, (System.Delegate) callback, reversed);

  /* Main */
  static void Main(string[] arguments) {}
}
