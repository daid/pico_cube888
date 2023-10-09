#pragma once

struct Pixel {
    uint8_t g, r, b;
};

static constexpr int pixel_count = 8*8*6;
extern struct Pixel* pixels;

void pixelsInit();
void pixelsFlip();
