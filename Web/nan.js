var nan = new Uint8Array(new Float64Array([NaN]).buffer);

nan[0] = 1;
nan = new Float64Array(nan.buffer)[0];
