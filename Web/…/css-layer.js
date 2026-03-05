/* THIS FUNCTION IS FAKE NEWS */
void function(layers) {
  for (var layersIndex = 1; layersIndex !== layers.length; ++layersIndex) {
    var order     = layers[layersIndex];
    var sublayers = [];

    console.log(layersIndex + " => [" + order.join(", ") + ']');
    // [0, 0.A, 2, 2.A, 2.B, 5, 5.A, 8, 8.A, 8.A.i, 9] [2, 8, 5.A, 0.A]
    // [0]
    // [0, 0.A]
    // [0, 2, 0.A]
    // [0, 2, 2.A, 0.A]
    // [0, 2, 2.A, 2.B, 0.A]
    // [0, 2, 2.A, 2.B, 0.A, 5]
    // [0, 2, 2.A, 2.B, 5.A, 0.A, 5]
    // [0, 2, 2.A, 2.B, 8, 5.A, 0.A, 5]
    // [0, 2, 2.A, 2.B, 8, 8.A, 5.A, 0.A, 5]
    // [0, 2, 2.A, 2.B, 8, 8.A, 8.A.i, 5.A, 0.A, 5]
    // [0, 2, 2.A, 2.B, 8, 8.A, 8.A.i, 5.A, 0.A, 5, 9] [0, 9, 5]
    // [0]
    // [0, 2]
    // [0, 2, 2.A]
    // [0, 2, 2.A, 2.B]
    // [0, 2, 2.A, 2.B, 8]
    // [0, 2, 2.A, 2.B, 8, 8.A]
    // [0, 2, 2.A, 2.B, 8, 8.A, 8.A.i]
    // [0, 2, 2.A, 2.B, 8, 8.A, 8.A.i, 5.A]
    // [0, 2, 2.A, 2.B, 8, 8.A, 8.A.i, 0.A, 5.A]
    // [0, 2, 2.A, 2.B, 8, 8.A, 8.A.i, 0.A, 5.A, 5]
    // [0, 2, 2.A, 2.B, 8, 8.A, 8.A.i, 0.A, 9, 5.A, 5]

    // [0, 2, 2.A, 2.B, 8, 8.A, 8.A.i, 0.A, 9, 5, 5.A]
    // ...
    for (var COUNT = layersIndex > 1 ? 11 : Infinity, index = 0; COUNT-- && index !== layers[0].length; ++index) {
      var layer   = layers[0][index];
      var ordered = false;

      // ...
      for (var orderIndex = order.length; orderIndex--; )
      if (layer === order[orderIndex] || 0 === layer.indexOf(order[orderIndex] + '.')) {
        for (var subindex = 0; !ordered && subindex !== sublayers.length; ++subindex)
        for (var suborderIndex = order.length; orderIndex !== --suborderIndex; )
        if (order[suborderIndex] === sublayers[subindex] || 0 === sublayers[subindex].indexOf(order[suborderIndex] + '.')) {
          ordered = true;
          void sublayers.splice(subindex, 0, layer);

          break
        }

        break
      }

      if (!ordered)
      void sublayers.push(layer);

      console.log("  [" + sublayers.join(', ') + ']')
    }

    layers[0] = sublayers
  }
}([["0", "0.A", "2", "2.A", "2.B", "5", "5.A", "8", "8.A", "8.A.i", "9"], ["2", "8", "5.A", "0.A"], ["0", "9", "5"]])
