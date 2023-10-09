#pragma once

#include "math.h"

struct Point {
    float x, y, z;

    float mag() const { return sqrtf(x*x+y*y+z*z); }
    Point norm() const {
        auto m = mag();
        return {x/m, y/m, z/m};
    }
    float dot(const Point& p) const {
        return x*p.x+y*p.y+z*p.z;
    }

    Point& operator+=(const Point& p) {
        x += p.x;
        y += p.y;
        z += p.z;
        return *this;
    }
    Point operator+(const Point& p) {
        return {x+p.x, y+p.y, z+p.z};
    }
    Point operator*(float f) {
        return {x*f, y*f, z*f};
    }
};
