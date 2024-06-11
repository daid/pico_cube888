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

class ALP {
public:
    ALP() = default;
    ALP(int idx) : idx(idx) {
        up = idx_to_normal(idx);
        forward = up.x == 0.0 ? Point{1, 0, 0} : Point{0, 1, 0};
        side = up.cross(forward);
    }

    ALP next(int dir) {
        dir = dir & 3;
        Point p;
        switch(dir) {
        case 0: p = idx_to_point(idx) + forward * .25; break;
        case 1: p = idx_to_point(idx) + side * .25; break;
        case 2: p = idx_to_point(idx) - forward * .25; break;
        case 3: p = idx_to_point(idx) - side * .25; break;
        }
        ALP result;
        result.idx = point_to_idx(p);
        result.up = idx_to_normal(result.idx);
        if (result.up == up) {
            result.forward = forward;
            result.side = side;
        } else if (result.up == forward) {
            result.forward = -up;
            result.side = side;
        } else if (result.up == -forward) {
            result.forward = up;
            result.side = side;
        } else if (result.up == side) {
            result.forward = forward;
            result.side = -up;
        } else if (result.up == -side) {
            result.forward = forward;
            result.side = up;
        } else {
            printf("Uh?\n");
        }
        return result;
    }

    void clearUp(Point& p) {
        if (up.x) p.x = 0;
        if (up.y) p.y = 0;
        if (up.z) p.z = 0;
    }

    ALP nextTowards(Point& p) {
        auto fwd_mag = (p - forward).mag();
        auto side_mag = (p - side).mag();
        auto back_mag = (p + forward).mag();
        auto other_mag = (p + side).mag();

        if (fwd_mag < side_mag && fwd_mag < back_mag && fwd_mag < other_mag) {
            p -= forward;
            return next(0);
        }
        if (side_mag < back_mag && side_mag < other_mag) {
            p -= side;
            return next(1);
        }
        if (back_mag < other_mag) {
            p += forward;
            return next(2);
        }
        p += side;
        return next(3);
    }

    int idx;
    Point up, forward, side;
};

int blocks[7][4] = {
    {0, 2, 2, 2},
    {0, 2, 0, 1},
    {0, 2, 2, 1},
    {2, 1, 2, 1},
    {0, 1, 1, 2},
    {0, 2, 1, 4},
    {2, 1, 1, 0},
};
Pixel block_color[7] = {
    {5, 0, 5},
    {0, 0,10},
    {5, 5, 5},
    {5, 5, 0},
    {10, 0, 0},
    {0, 5, 5},
    {0,10, 0},
};
uint8_t grid[8*8*6] = {0};


int main() {
    stdio_init_all();
    sleep_ms(500);
    printf("start\n");

    accelInit();
    printf("accelInit:done\n");
    pixelsInit();
    printf("pixelsInit:done\n");
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    /*
    while(1) {
        sleep_ms(100);
        pixels[0] = {255, 255, 255};
        pixels[4] = {255, 0, 0};
        pixels[32] = {0, 255, 0};
        pixels[36] = {0, 0, 255};
        pixelsFlip();
    }
    return 0;
    */

    Point offset = {0.0, 0.0, 0.0};
    ALP center(36);
    int block_id = 6;
    grid[2*64+36] = 3;
    while(1) {
        auto a = accelRead().norm();
        offset += a * 0.03;
        for(int n=0; n<8*8*6; n++) {
            //auto p = idx_to_point(n);
            //auto f = p.dot(a);
            //pixels[n].r = std::clamp(f * 5, 0.0f, 20.0f);
            //pixels[n].g = std::clamp(f *-5, 0.0f, 20.0f);
            //pixels[n] = hsv(atan2f(p.y, p.x) / M_PI * 180.0, f * -0.5 + 0.5, 0.05);
            //p = p * 40 + offset;
            //pixels[n] = hsv(noise.GetNoise(p.x, p.y, p.z) * 180.0, 1.0, .05);
            if (grid[n])
                pixels[n] = block_color[grid[n]-1];
            else
                pixels[n] = {0, 0, 0};
        }
        for(int n=0; n<3; n++) {
            auto next = center.next(blocks[block_id][n]);
            if (n == 2 && blocks[block_id][3] != 4) next = next.next(blocks[block_id][3]);
            pixels[next.idx] = block_color[block_id];
        }
        pixels[center.idx] = block_color[block_id];
        pixelsFlip();

        center.clearUp(offset);
        if (offset.mag() > 1.0) {
            auto new_center = center.nextTowards(offset);
            bool valid = true;
            for(int n=0; n<3; n++) {
                auto next = new_center.next(blocks[block_id][n]);
                if (n == 2 && blocks[block_id][3] != 4) next = next.next(blocks[block_id][3]);
                if (grid[next.idx]) valid = false;
            }
            if (grid[new_center.idx]) valid = false;

            if (valid) {
                center = new_center;
            } else {
                for(int n=0; n<3; n++) {
                    auto next = center.next(blocks[block_id][n]);
                    if (n == 2 && blocks[block_id][3] != 4) next = next.next(blocks[block_id][3]);
                    grid[next.idx] = block_id + 1;
                }
                grid[center.idx] = block_id + 1;

                center = ALP(36);
                block_id = (block_id + accelRawRng()) % 7;
            }
        }
    }

    return 0;
}

