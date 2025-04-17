public static class Program {
  public delegate void name(name _);

  /* Main */
  static void Main(string[] arguments) {
    var me = (name) (static myself => myself(myself));
    me(me);
  }
}
