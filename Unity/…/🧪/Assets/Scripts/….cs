public static class Program {
  /* Main --> del ….txt ….xml & cls && Unity -projectPath . -batchmode -logFile ….txt -runTests -testPlatform PlayMode -testResults ….xml -quit & (findstr /R /C:".*\.cs(" /C:"error CS" /C:"warning CS" ….txt || echo Compiled successfully) & del ….txt ….xml */
  [UnityEngine.RuntimeInitializeOnLoadMethod]
  private static void Main() => new UnityEngine.GameObject("…", typeof(ProgramBehaviour));
}

internal sealed class ProgramBehaviour : UnityEngine.MonoBehaviour {
  private void Awake() {
    /* Do something… */
    System.Runtime.CompilerServices.ITuple tuple = (1, 2);

    // new System.Collections.ObjectModel.ReadOnlySet<T>();
    System.Array.Fill(new int[3], 42);
  }

  private void FixedUpdate() {}
  private void OnDestroy  () {}
}
