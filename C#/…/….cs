#nullable enable annotations

public class Program {
  public class A {
    public void foo() {}
  }

  /* Main --> del ….exe & cls && csc /out:….exe /t:exe ….cs && ….exe & del ….exe */
  static void Main(string[] arguments) {
    System.Action? _ = (null as A)?.foo ?? () => {};
  }
}
