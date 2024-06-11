#include "cube.h"
#include <algorithm>


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

Point idx_to_normal(int idx)
{
    int side = (idx & 0xFC0) >> 6;
    switch(side) {
    case 0: return { 0, 0, 1.0f};
    case 1: return { 0,-1.0f, 0};
    case 2: return { 0, 0,-1.0f};
    case 3: return { 0, 1.0f, 0};
    case 4: return { 1.0f, 0, 0};
    case 5: return {-1.0f, 0, 0};
    default: return {0, 0, 0};
    }
}

static int xy_to_idx(int x, int y) {
    x = std::clamp(x, 0, 7);
    y = std::clamp(y, 0, 7);
    if (y & 1)
        return 7 - x + y * 8;
    return x + y * 8;
}

int point_to_idx(const Point& p) {
    auto ax = fabsf(p.x);
    auto ay = fabsf(p.y);
    auto az = fabsf(p.z);
    if (ax > ay && ax > az) {
        auto y = ((p.y / ax) + 1.0) * 4.0;
        auto z = ((p.z / ax) + 1.0) * 4.0;
        if (p.x > 0)
            return xy_to_idx(int(z), int(y)) + 256;
        return xy_to_idx(int(z), 7-int(y)) + 320;
    } else if (ay > az) {
        auto x = ((p.x / ay) + 1.0) * 4.0;
        auto z = ((p.z / ay) + 1.0) * 4.0;
        if (p.y > 0.0)
            return xy_to_idx(int(z), 7-int(x)) + 192;
        return xy_to_idx(7-int(z), 7-int(x)) + 64;
    } else if (az == 0.0) {
        return 0;
    } else {
        auto x = ((p.x / az) + 1.0) * 4.0;
        auto y = ((p.y / az) + 1.0) * 4.0;
        if (p.z > 0)
            return xy_to_idx(int(y), int(x)) + 0;
        return xy_to_idx(int(y), 7-int(x)) + 128;
    }
}
