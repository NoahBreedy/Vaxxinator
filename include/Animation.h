#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

class Animation {
public:
    Animation(SDL_Renderer* renderer,
              const std::string& pngPath,
              int frameW,
              int frameH,
              int framesPerStep,
              int frameCount = -1,
              bool looping = true);

    ~Animation();

    bool ok() const { return texture != nullptr; }

    void setLooping(bool loop) { looping = loop; }
    bool isLooping() const { return looping; }

    void reset(bool immediate = true);

    void update();
    bool isFinished() const { return finished; }

    SDL_Texture* getTexture() const { return texture; }
    SDL_Rect getSrcRect() const;

    int getFrameW() const { return frameW; }
    int getFrameH() const { return frameH; }
    int getFrameCount() const { return frameCount; }

private:
    SDL_Texture* texture = nullptr;

    int texW = 0;
    int texH = 0;

    int frameW = 0;
    int frameH = 0;
    int frameCount = 1;

    int framesPerStep = 1;
    int tick = 0;
    int frameIndex = 0;

    bool looping = true;
    bool finished = false;
};
