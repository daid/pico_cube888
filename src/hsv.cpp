#include "hsv.h"
#include <math.h>
#include <algorithm>


Pixel hsv(float h, float s, float v) {
    while(h < 0.0) h += 360.0f;
    while(h > 360.0) h -= 360.0f;
    auto c = v * s;
    auto x = c * (1.0 - fabsf(fmodf(h / 60, 2) - 1.0));
    auto m = v - c;
    if (h < 60) return {
        uint8_t(std::clamp(int((m+x)*255), 0, 255)),
        uint8_t(std::clamp(int((m+c)*255), 0, 255)),
        uint8_t(std::clamp(int((m+0)*255), 0, 255)),
    };
    if (h < 120) return {
        uint8_t(std::clamp(int((m+c)*255), 0, 255)),
        uint8_t(std::clamp(int((m+x)*255), 0, 255)),
        uint8_t(std::clamp(int((m+0)*255), 0, 255)),
    };
    if (h < 180) return {
        uint8_t(std::clamp(int((m+c)*255), 0, 255)),
        uint8_t(std::clamp(int((m+0)*255), 0, 255)),
        uint8_t(std::clamp(int((m+x)*255), 0, 255)),
    };
    if (h < 240) return {
        uint8_t(std::clamp(int((m+x)*255), 0, 255)),
        uint8_t(std::clamp(int((m+0)*255), 0, 255)),
        uint8_t(std::clamp(int((m+c)*255), 0, 255)),
    };
    if (h < 300) return {
        uint8_t(std::clamp(int((m+0)*255), 0, 255)),
        uint8_t(std::clamp(int((m+x)*255), 0, 255)),
        uint8_t(std::clamp(int((m+c)*255), 0, 255)),
    };
    return {
        uint8_t(std::clamp(int((m+0)*255), 0, 255)),
        uint8_t(std::clamp(int((m+c)*255), 0, 255)),
        uint8_t(std::clamp(int((m+x)*255), 0, 255)),
    };
}
