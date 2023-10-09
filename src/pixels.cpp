#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/dma.h"
#include "neopixel.pio.h"
#include "pixels.h"

static struct Pixel buffer0[pixel_count];
static struct Pixel buffer1[pixel_count];
struct Pixel* pixels = buffer0;

static constexpr int pin = 29;

void pixelsInit() {
    neopixel_program_init(pio0, 0, pio_add_program(pio0, &neopixel_program), pin);
    pio_sm_set_enabled(pio0, 0, true);

    dma_channel_config channel_config = dma_channel_get_default_config(0);
    channel_config_set_dreq(&channel_config, pio_get_dreq(pio0, 0, true));
    channel_config_set_transfer_data_size(&channel_config, DMA_SIZE_8);
    channel_config_set_read_increment(&channel_config, true);
    dma_channel_configure(0,
                        &channel_config,
                        &pio0->txf[0],
                        pixels,
                        sizeof(buffer0),
                        true);
}

void pixelsFlip()
{
    while(dma_channel_is_busy(0)) {}
    sleep_us(200);
    dma_channel_set_read_addr(0, pixels, true);
    if (pixels == buffer0) {
        pixels = buffer1;
    } else {
        pixels = buffer0;
    }
}
