using static Program.Permissions;

public class Program {
  [System.Flags]
  public enum Permissions : byte { None = (byte) 0x0u, Read = (byte) 0x1u, Write = (byte) 0x2u, Execute = (byte) 0x4u };

  /* Main --> del ….exe & cls && csc /out:….exe /t:exe ….cs && ….exe & del ….exe */
  static void Main(string[] arguments) {
    Permissions permissions = Read | Write;

    System.Console.WriteLine($"[RWE]: {permissions & Read} {permissions & Write} {permissions & Execute}");
    permissions |= Execute;
    System.Console.WriteLine($"[RWE]: {permissions & Read} {permissions & Write} {permissions & Execute}");
    permissions &= ~Write;
    System.Console.WriteLine($"[RWE]: {permissions & Read} {permissions & Write} {permissions & Execute}");
  }
}
