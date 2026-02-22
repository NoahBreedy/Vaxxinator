#include "core/Utils.h"

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

nfdchar_t* openFileDialog() {
        NFD::Guard nfdGuard;

        // auto-freeing memory
        NFD::UniquePath outPath;

        // prepare filters for the dialog
        nfdfilteritem_t filterItem[2] = {{"Test", "txt"}, {"Binary", "bin"}};

        nfdchar_t* filePath = nullptr;

        // show the dialog
        nfdresult_t result = NFD::OpenDialog(outPath, filterItem, 2);
        if (result == NFD_OKAY) {
            filePath = outPath.get();
        } else if (result == NFD_CANCEL) {
            std::cout << "User pressed cancel." << std::endl;
        } else {
            std::cout << "Error: " << NFD::GetError() << std::endl;
        }

    return filePath;
}
