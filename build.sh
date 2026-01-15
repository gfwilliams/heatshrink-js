#!/bin/bash

# git clone https://github.com/atomicobject/heatshrink
cp heatshrink_config.h heatshrink/heatshrink_config.h

echo Running emscripten
emcc heatshrink_wrapper.c heatshrink/heatshrink_encoder.c heatshrink/heatshrink_decoder.c -o heatshrink_wrapper.out.js -s EXPORTED_FUNCTIONS='["_compress","_decompress","_malloc","_free"]' -s EXTRA_EXPORTED_RUNTIME_METHODS='["ccall"]' -s WASM=0 -Oz

echo Adding extra code
cat heatshrink_notes.js > heatshrink.js
echo "/*" >> heatshrink.js
cat heatshrink/LICENSE >> heatshrink.js
echo "*/" >> heatshrink.js
cat heatshrink_wrapper.pre.js >> heatshrink.js
cat heatshrink_wrapper.out.js >> heatshrink.js
cat heatshrink_wrapper.post.js >> heatshrink.js

echo heatshrink.js created
