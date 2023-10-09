#include <stdio.h>
#include <string.h>
#include <math.h>
#include <algorithm>
#include "pico/stdlib.h"
#include "pixels.h"
#include "accel.h"


Point idx_to_point(int idx) {
    int side = (idx & 0xFC0) >> 6;
    float x = ((idx & 0x07) + 0.5f) / 4.0f - 1.0f;
    float y = (((idx & 0x38) >> 3) + 0.5f) / 4.0f - 1.0f;
    if (idx & 0x08) x = -x;
    switch(side) {
    case 0: return { y, x, 1.0f};
    case 1: return {-y,-1.0f, -x};
    case 2: return {-y, x,-1.0f};
    case 3: return {-y, 1.0f, x};
    case 4: return { 1.0f, y, x};
    case 5: return {-1.0f, -y, x};
    default: return {0, 0, 0};
    }
}


int main() {
    stdio_init_all();
    sleep_ms(500);

    accelInit();
    pixelsInit();

    while(1) {
        auto a = accelRead().norm();
        for(int n=0; n<8*8*6; n++) {
            auto p = idx_to_point(n);
            auto f = p.dot(a);
            pixels[n].r = std::clamp(f * 5, 0.0f, 20.0f);
            pixels[n].g = std::clamp(f *-5, 0.0f, 20.0f);
        }
        pixelsFlip();
    }

    return 0;
}
