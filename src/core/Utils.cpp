#include <cstdint>
#include <cstdlib>
#include <cmath>

/* CHAT-GPT hsv to rgb function (i'll verify it later) */
void hsv_to_rgb(float h, float s, float v, uint8_t& r, uint8_t& g, uint8_t& b) {
    float c = v * s;
    float x = c * (1 - std::fabs(std::fmod(h / 60.0f, 2) - 1));
    float m = v - c;

    float rf, gf, bf;

    if (h < 60)       { rf = c; gf = x; bf = 0; }
    else if (h < 120) { rf = x; gf = c; bf = 0; }
    else if (h < 180) { rf = 0; gf = c; bf = x; }
    else if (h < 240) { rf = 0; gf = x; bf = c; }
    else if (h < 300) { rf = x; gf = 0; bf = c; }
    else              { rf = c; gf = 0; bf = x; }

    r = static_cast<uint8_t>((rf + m) * 255);
    g = static_cast<uint8_t>((gf + m) * 255);
    b = static_cast<uint8_t>((bf + m) * 255);
}

float rand_float(float min, float max) {
    float scale = (float)std::rand() / (float)RAND_MAX;
    return min + scale * (max - min);
}

double distance(int x1, int y1, int x2, int y2) {
    return std::sqrt(std::pow(x1-x2,2) + std::pow(y1-y2,2));
}