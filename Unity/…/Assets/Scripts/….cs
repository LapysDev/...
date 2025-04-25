public static class Program {
  [UnityEngine.RuntimeInitializeOnLoadMethod]
  private static void Main() => new UnityEngine.GameObject("…", typeof(ProgramBehaviour));
}

internal sealed class ProgramBehaviour : UnityEngine.MonoBehaviour {
  /* … */
  private void Awake() {
    /* Do something… */
  }

  private void FixedUpdate() {}
  private void OnDestroy  () {}
}
