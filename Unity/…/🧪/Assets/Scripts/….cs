public static class Program {
  /* Main --> del ….txt ….xml & cls && Unity -projectPath . -batchmode -logFile ….txt -nographics -runTests -testPlatform EditMode -testResults ….xml -quit & (findstr /R /C:".*\.cs(" /C:"error CS" /C:"warning CS" ….txt || echo Compiled successfully) & del ….txt ….xml */
  [UnityEngine.RuntimeInitializeOnLoadMethod]
  private static void Main() => new UnityEngine.GameObject("…", typeof(ProgramBehaviour));
}

internal sealed class Meh : UnityEngine.MonoBehaviour {
  internal UnityEngine.Transform _transform = null!;
  internal new ref readonly UnityEngine.Transform transform { get { this._transform = null == this._transform ? base.transform : this._transform; return ref this._transform; } }
}

internal sealed class ProgramBehaviour : UnityEngine.MonoBehaviour {
  private void Awake() {
    Meh meh = new UnityEngine.GameObject("meh").AddComponent<Meh>();
    meh.GetComponent<UnityEngine.Transform>().localPosition += UnityEngine.Vector3.one * 2.0f;
    UnityEngine.Debug.Log($"{meh.transform.position} {meh.GetComponent<UnityEngine.Transform>().position}");
  }
}
