void notmain() {
  struct T {
    friend void ::notmain();
    private:
      ~T() {}
  } foo = {};
  (void) foo; // only `notmain()` can destroy `foo`
}
