#include <stdio.h>
#include "heatshrink/heatshrink_encoder.h"
#include "heatshrink/heatshrink_decoder.h"

#undef ASSERT
#define ASSERT(A) if (!(A)) { printf("Assert at heatshrink_wrapper.c:%d\n", __LINE__); return 0; }
#define ASSERT_EQ(A,B) ASSERT((A) == (B));

int compress(uint8_t *input, uint32_t input_size, uint8_t *output, uint32_t output_size, int log_lvl) {
    heatshrink_encoder hse;
    heatshrink_encoder_reset(&hse);
    size_t count = 0;

    if (log_lvl > 1) {
        printf("\n^^ COMPRESSING %d bytes\n", input_size);
    }

    uint32_t sunk = 0;
    uint32_t polled = 0;
    while (sunk < input_size) {
        ASSERT(heatshrink_encoder_sink(&hse, &input[sunk], input_size - sunk, &count) >= 0);
        sunk += count;
        if (log_lvl > 1) printf("^^ sunk %zd\n", count);
        if (sunk == input_size) {
            ASSERT_EQ(HSER_FINISH_MORE, heatshrink_encoder_finish(&hse));
        }

        HSE_poll_res pres;
        do {                    /* "turn the crank" */
            if (output) {
              pres = heatshrink_encoder_poll(&hse, &output[polled], output_size - polled, &count);
            } else {
              uint8_t buf[64];              
              pres = heatshrink_encoder_poll(&hse, buf, sizeof(buf), &count);
            }
            ASSERT(pres >= 0);
            polled += count;
            if (log_lvl > 1) printf("^^ polled %zd\n", count);
        } while (pres == HSER_POLL_MORE);
        ASSERT_EQ(HSER_POLL_EMPTY, pres);
        if (sunk == input_size) {
            ASSERT_EQ(HSER_FINISH_DONE, heatshrink_encoder_finish(&hse));
        }
    }
    if (log_lvl > 0) printf("in: %u compressed: %u\n", input_size, polled);
    uint32_t compressed_size = polled;

    return compressed_size;
}

int decompress(uint8_t *input, uint32_t input_size, uint8_t *output, uint32_t output_size, int log_lvl) {
    heatshrink_decoder hsd;
    heatshrink_decoder_reset(&hsd);
    size_t count = 0;

    if (log_lvl > 1) {
        printf("\n^^ DECOMPRESSING %d bytes\n", input_size);
    }

    uint32_t sunk = 0;
    uint32_t polled = 0;
    while (sunk < input_size) {
        ASSERT(heatshrink_decoder_sink(&hsd, &input[sunk], input_size - sunk, &count) >= 0);
        sunk += count;
        if (log_lvl > 1) printf("^^ sunk %zd\n", count);
        if (sunk == input_size) {
            ASSERT_EQ(HSER_FINISH_MORE, heatshrink_decoder_finish(&hsd));            
        }

        HSD_poll_res pres;
        do {                    /* "turn the crank" */
            if (output) {
              pres = heatshrink_decoder_poll(&hsd, &output[polled], output_size - polled, &count);
            } else {
              uint8_t buf[64];              
              pres = heatshrink_decoder_poll(&hsd, buf, sizeof(buf), &count);
            }
            ASSERT(pres >= 0);
            polled += count;
            if (log_lvl > 1) printf("^^ polled %zd\n", count);
        } while (pres == HSDR_POLL_MORE && count!=0);
        ASSERT_EQ(HSDR_POLL_EMPTY, pres);
        if (sunk == input_size) {
            ASSERT_EQ(HSDR_FINISH_DONE, heatshrink_decoder_finish(&hsd));
        }
    }
    if (log_lvl > 0) printf("in: %u compressed: %u\n", input_size, polled);
    uint32_t compressed_size = polled;

    return compressed_size;
}
