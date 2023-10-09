#include <stdio.h>
#include <string.h>
#include <math.h>
#include <algorithm>
#include "pico/stdlib.h"
#include "pixels.h"
#include "accel.h"
#include "cube.h"
#include "hsv.h"
#include "fastnoiselite.h"


FastNoiseLite noise;


int main() {
    stdio_init_all();
    sleep_ms(500);

    accelInit();
    pixelsInit();
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

    Point offset = {0.0, 0.0, 0.0};
    while(1) {
        auto a = accelRead().norm();
        offset += a;
        for(int n=0; n<8*8*6; n++) {
            auto p = idx_to_point(n);
            //auto f = p.dot(a);
            //pixels[n].r = std::clamp(f * 5, 0.0f, 20.0f);
            //pixels[n].g = std::clamp(f *-5, 0.0f, 20.0f);
            //pixels[n] = hsv(atan2f(p.y, p.x) / M_PI * 180.0, f * -0.5 + 0.5, 0.05);
            p = p * 40 + offset;
            pixels[n] = hsv(noise.GetNoise(p.x, p.y, p.z) * 180.0, 1.0, 0.1);
        }
        pixelsFlip();
    }

    return 0;
}

