/* Event > ... > Message */
void (self.onmessage = function message(event) {
  var pictureData = event.data;

  // ...
  function assembled(module) {
    with (module.instance.exports) try {
      var RGBAArray = "function" !== typeof Uint8ClampedArray ? Uint8Array  : Uint8ClampedArray;
      var SizeArray = "function" !== typeof BigUint64Array    ? Uint32Array : BigUint64Array;

      var count = null, counts = null, data = null, values = null;
      var capacity = memory.buffer.byteLength, offset = 0x0, size = (
        SizeArray.BYTES_PER_ELEMENT   + SizeArray.BYTES_PER_ELEMENT                                + // --> count
        RGBAArray.BYTES_PER_ELEMENT   + (RGBAArray.BYTES_PER_ELEMENT   * (pictureData.length / 1)) + // --> data
        SizeArray.BYTES_PER_ELEMENT   + (SizeArray.BYTES_PER_ELEMENT   * (pictureData.length / 4)) + // --> counts
        Uint32Array.BYTES_PER_ELEMENT + (Uint32Array.BYTES_PER_ELEMENT * (pictureData.length / 4)) + // --> values
        +0
      );

      // ...
      if (size > capacity) memory.grow((size - capacity) / (/* ->> Page byte size */ 64 * 1024));
      count  = new SizeArray  (memory.buffer, offset += +0                + (+0                % SizeArray.BYTES_PER_ELEMENT),   1); // ->> pointer
      counts = new SizeArray  (memory.buffer, offset += count.byteLength  + (count.byteLength  % SizeArray.BYTES_PER_ELEMENT),   pictureData.length / 4);
      data   = new RGBAArray  (memory.buffer, offset += counts.byteLength + (counts.byteLength % RGBAArray.BYTES_PER_ELEMENT),   pictureData.length / 1);
      values = new Uint32Array(memory.buffer, offset += data.byteLength   + (data.byteLength   % Uint32Array.BYTES_PER_ELEMENT), pictureData.length / 4);

      count[0] = count[0].constructor(+0);
      data.set(pictureData);

      // ...
      count = Number(extrapolate(values.byteOffset, counts.byteOffset, count.byteOffset, data.byteOffset, data.length, null, null, +0, SizeArray.BYTES_PER_ELEMENT * 8));
      self.postMessage({"counts": counts.subarray(+0, count), "length": Math.min(count, /* --> Number.MAX_SAFE_INTEGER */ 9007199254740991), "values": values.subarray(+0, count)})
    } catch (error) { interpreted() }
  }

  function interpreted() {
    console.warn("[interpreted]");
    self.postMessage({"counts": null, "length": +0, "values": null})
  }

  // ...
  // self.onmessage = function message(event) { assembled(module) };

  if ("object" !== typeof WebAssembly)
    interpreted();

  else if ("function" === typeof fetch && "function" === typeof WebAssembly.instantiateStreaming) {
    var request = fetch("extrapolate.wasm", {"headers": {"Content-Type": "application/wasm"}});
    request.then(function(response) { response.status > 199 && response.status < 300 ? WebAssembly.instantiateStreaming(request, {}).then(assembled).catch(interpreted) : interpreted() }).catch(interpreted)
  }

  else {
    var request = new XMLHttpRequest;

    // ...
    request.onerror      = function error(event) { interpreted() };
    request.onload       = function load(event) { if (0x4 /* XMLHttpRequest.DONE */ === request.readyState) request.status > 199 && request.status < 300 ? WebAssembly.instantiate(request.response, {}).then(assembled).catch(interpreted) : interpreted() };
    request.responseType = "arraybuffer";

    request.open("GET", "extrapolate.wasm", true, null, null);
    request.setRequestHeader("Content-Type", "application/wasm");
    request.send(null)
  }
});
