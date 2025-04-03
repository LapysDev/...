public class ECS {
  private sealed class WaitBehaviour : UnityEngine.MonoBehaviour {}

  private sealed class WaitInfo {
    internal readonly struct HandlerInfo {
      internal readonly ECS.Handler value;
      internal readonly object?     metadata;

      internal HandlerInfo(ECS.Handler value, object? metadata) {
        this.metadata = metadata;
        this.value    = value;
      }
    }

    internal UnityEngine.Coroutine                        coroutine = null!;
    internal System.Collections.Generic.List<HandlerInfo> handlers  = new(1); // ⟶ `event` with extra steps…
  }

  public delegate void Handler(object? _);

  /* … */
  private static UnityEngine.MonoBehaviour                                   WAIT  = new UnityEngine.GameObject("…").AddComponent<ECS.WaitBehaviour>();
  private static System.Collections.Generic.Dictionary<double, ECS.WaitInfo> WAITS = new(16);

  /* … */
  public static uint Unwait() {
    uint     count     = 0u;
    double[] durations = new double[WAITS.Count];

    // …
    WAITS.Keys.CopyTo(durations, 0);

    foreach (double delay in durations)
    foreach (WAITS[delay])
      count += Unwait(delay, callback);

    return count;
  }

  public static uint Unwait(ECS.Handler callback) {
    uint     count     = 0u;
    double[] durations = new double[WAITS.Count];

    // …
    WAITS.Keys.CopyTo(durations, 0);

    foreach (double delay in durations)
      count += Unwait(delay, callback);

    return count;
  }

  public static uint Unwait(double delay, ECS.Handler callback) {
    uint count = 0u;

    // …
    if (WAITS.TryGetValue(delay, out ECS.WaitInfo wait)) {
      for (int index = wait.handlers.Count; 0 != index--; )
      if (callback == wait.handlers[index].value) {
        ++count;
        wait.handlers.RemoveAt(index);
      }

      if (0 == wait.handlers.Count)
      WAITS.Remove(delay);
    }

    return count;
  }

  public static void Wait(double delay, ECS.Handler callback, uint count, object? data = null) {
    static System.Collections.IEnumerator EnumerateRoutine(double delay, uint count) {
      bool         forever = 0u == count;
      ECS.WaitInfo wait    = WAITS[delay];

      // …
      while (forever || 0u != count--) {
        yield return new UnityEngine.WaitForSecondsRealtime((float) delay);

        if (0 == wait.handlers.Count)
        break;

        foreach (var handler in wait.handlers)
        handler.value(handler.metadata);
      }

      WAIT.StopCoroutine(wait.coroutine);
    }

    if (!WAITS.TryGetValue(delay, out ECS.WaitInfo wait)) {
      WAITS.Add(delay, wait = new());

      wait.handlers  = new(1) {new(value: callback, metadata: data)};
      wait.coroutine = WAIT.StartCoroutine(EnumerateRoutine(delay, count));
    } else wait.handlers.Add(new(value: callback, metadata: data));
  }

  public static void WaitEvery(double delay, ECS.Handler callback, object? data = null) => Wait(delay, callback, 0u, data);
  public static void WaitUntil(double delay, ECS.Handler callback, object? data = null) => Wait(delay, callback, 1u, data);

  [UnityEngine.RuntimeInitializeOnLoadMethod]
  private static void Main() {
    ECS.Handler bro   = null!;
    ECS.Handler bruh  = null!;
    int             count = 3;

    UnityEngine.Object.DontDestroyOnLoad(ECS.WAIT);

    UnityEngine.Debug.Log("[A]: Fire immediately");
      ECS.WaitUntil(3.0, static _ => UnityEngine.Debug.Log("[C]: Fired after 3 seconds"));

      ECS.WaitEvery(5.0, bruh = n => { UnityEngine.Debug.Log($"[D]: Fired every {n} seconds"); if (0 == count--) ECS.Unwait(5.0, bruh); }, 5.1);
      ECS.WaitEvery(7.0, bruh, 7.1);

      ECS.WaitUntil(0.0, bro = static _ => UnityEngine.Debug.Log("[]: Never called?"));
      ECS.Unwait   (bro);
    UnityEngine.Debug.Log("[B]: Fire afterward");
  }
}
