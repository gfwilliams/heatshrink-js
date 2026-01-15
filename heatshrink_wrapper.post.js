var HS_LOG_LEVEL = 0;

function heatshrink_compress(inputBuffer) {
        if (inputBuffer.BYTES_PER_ELEMENT!=1) throw new Error("Expecting Byte Array");
        var input_size = inputBuffer.length;
        var output_size = input_size + (input_size/2) + 4;;

        var bufIn = Module._malloc(input_size);
        Module.HEAPU8.set(inputBuffer, bufIn);
        // int compress(uint8_t *input, uint32_t input_size, uint8_t *output, uint32_t output_size, int log_lvl)
        output_size = Module.ccall('compress', 'number', ['number','number','number','number','number'], [bufIn,input_size,0,0,HS_LOG_LEVEL/*log level*/])+1;
        var bufOut = Module._malloc(output_size);
        output_size = Module.ccall('compress', 'number', ['number','number','number','number','number'], [bufIn,input_size,bufOut,output_size,HS_LOG_LEVEL/*log level*/]);
        // console.log("Compressed to "+output_size);

        var outputBuffer = new Uint8Array(output_size);
        for (var i=0;i<output_size;i++) outputBuffer[i]=Module.HEAPU8[bufOut+i];

        Module._free(bufIn);
        Module._free(bufOut);

        return outputBuffer;
}

function heatshrink_decompress(inputBuffer) {
        if (inputBuffer.BYTES_PER_ELEMENT!=1) throw new Error("Expecting Byte Array");
        var input_size = inputBuffer.length;
        var output_size = input_size + (input_size/2) + 4;;

        var bufIn = Module._malloc(input_size);
        Module.HEAPU8.set(inputBuffer, bufIn);
        // int compress(uint8_t *input, uint32_t input_size, uint8_t *output, uint32_t output_size, int log_lvl)
        output_size = Module.ccall('decompress', 'number', ['number','number','number','number','number'], [bufIn,input_size,0,0,HS_LOG_LEVEL/*log level*/])+1;
        var bufOut = Module._malloc(output_size);
        output_size = Module.ccall('decompress', 'number', ['number','number','number','number','number'], [bufIn,input_size,bufOut,output_size,HS_LOG_LEVEL/*log level*/]);
        // console.log("Compressed to "+output_size);

        var outputBuffer = new Uint8Array(output_size);
        for (var i=0;i<output_size;i++) outputBuffer[i]=Module.HEAPU8[bufOut+i];

        Module._free(bufIn);
        Module._free(bufOut);

        return outputBuffer;
}

return {
  compress : heatshrink_compress,
  decompress : heatshrink_decompress,
};
}));
