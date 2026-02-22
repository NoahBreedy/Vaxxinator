#ifndef __UTILS__
#define __UTILS__

#include <cstdint>
#include <cmath>
#include <iostream>
#include "nfd.hpp"

void hsv_to_rgb(float h, float s, float v, uint8_t& r, uint8_t& g, uint8_t& b);

nfdchar_t* openFileDialog();

#endif /* __UTILS__ */