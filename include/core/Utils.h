#ifndef __UTILS__
#define __UTILS__

#include <cstdint>

typedef struct Vec2 {
    int16_t x;
    int16_t y;
} Vec2;

typedef struct Vec2f {
    double   x;
    double   y;
} Vec2f;

typedef struct RGBA {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} RGBA;
#include <cmath>
#include <iostream>
#include "nfd.hpp"

void hsv_to_rgb(float h, float s, float v, uint8_t& r, uint8_t& g, uint8_t& b);

float rand_float(float min, float max);

double distance(int x1, int y1, int x2, int y2);

nfdchar_t* openFileDialog();

#endif /* __UTILS__ */