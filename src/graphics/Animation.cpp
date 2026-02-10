#include "graphics/Animation.h"
#include <iostream>

Animation::Animation(SDL_Renderer* renderer,
                     const std::string& pngPath,
                     int frameW,
                     int frameH,
                     int framesPerStep,
                     int frameCount,
                     bool looping)
    : frameW(frameW),
      frameH(frameH),
      framesPerStep(framesPerStep),
      looping(looping) {

    SDL_Surface* surface = IMG_Load(pngPath.c_str());
    if (!surface) {
        std::cerr << "IMG_Load failed (" << pngPath << "): " << IMG_GetError() << "\n";
        return;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        std::cerr << "SDL_CreateTextureFromSurface failed: " << SDL_GetError() << "\n";
        return;
    }

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    if (SDL_QueryTexture(texture, nullptr, nullptr, &texW, &texH) != 0) {
        std::cerr << "SDL_QueryTexture failed: " << SDL_GetError() << "\n";
        texW = texH = 0;
    }

    if (this->framesPerStep <= 0) this->framesPerStep = 1;

    if (frameCount <= 0) {
        this->frameCount = (frameW > 0) ? (texW / frameW) : 1;
    } else {
        this->frameCount = frameCount;
    }
    if (this->frameCount <= 0) this->frameCount = 1;

    reset(true);
}

Animation::~Animation() {
    if (texture) SDL_DestroyTexture(texture);
    texture = nullptr;
}

void Animation::reset(bool immediate) {
    finished = false;
    frameIndex = 0;
    tick = immediate ? (framesPerStep - 1) : 0;
    if (tick < 0) tick = 0;
}

void Animation::update() {
    if (finished) return;
    if (frameCount <= 1) return;

    tick++;
    if (tick < framesPerStep) return;

    tick = 0;

    if (frameIndex + 1 < frameCount) {
        frameIndex++;
    } else {
        if (looping) {
            frameIndex = 0;
        } else {
            frameIndex = frameCount - 1;
            finished = true;
        }
    }
}

SDL_Rect Animation::getSrcRect() const {
    SDL_Rect r;
    r.x = frameIndex * frameW;
    r.y = 0;
    r.w = frameW;
    r.h = frameH;
    return r;
}