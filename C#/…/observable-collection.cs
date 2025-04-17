public static class Program {
  /* Main */
  static void Main(string[] arguments) {
    System.Collections.ObjectModel.ObservableCollection<int> list = new(new System.Collections.Generic.List<int>(3) {1, 2, 3});
    list.CollectionChanged += static (target, notifyCollectionChangedEvent) => System.Console.WriteLine("[…]: List modified");
  }
}
