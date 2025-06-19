public static class Program {
  /* Main --> del ….txt ….xml & cls && Unity -projectPath . -batchmode -logFile ….txt -nographics -runTests -testPlatform EditMode -testResults ….xml -quit & (findstr /R /C:".*\.cs(" /C:"error CS" /C:"warning CS" ….txt || echo Compiled successfully) & del ….txt ….xml */
  [UnityEngine.RuntimeInitializeOnLoadMethod]
  private static void Main() => new UnityEngine.GameObject("…", typeof(ProgramBehaviour));
}

internal sealed class ProgramBehaviour : UnityEngine.MonoBehaviour {
  private void Awake() {
    /* Do something… */
  }

  private void FixedUpdate() {}
  private void OnDestroy  () {}
}
