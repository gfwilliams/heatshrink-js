Heatshrink.js
=============

An Emscripten port of the [heatshrink compression library](https://github.com/atomicobject/heatshrink) to JS.

This is built with [Espruino's](http://www.espruino.com) `heatshrink_config.h` to allow
compressed files to be created easily for it.

**A complete build is available at http://www.espruino.com/js/heatshrink.js**

To build, [install emscripten](https://emscripten.org/docs/getting_started/downloads.html), then:

```
git clone https://github.com/atomicobject/heatshrink
./build.sh
```

Using:
--------------


```
<script src="http://www.espruino.com/js/heatshrink.js"></script>
<script>
var input = new Uint8Array(32);
// ...
var compressed = heatshrink_compress(input);
// compressed is a Uint8Array
var decompressed = heatshrink_decompress(compressed);
// decompressed is a Uint8Array
</script>
```
