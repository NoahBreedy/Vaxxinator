#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <unordered_map>
#include <memory>

class Animation;

class Sprite {
public:
    explicit Sprite(SDL_Renderer* renderer);
    ~Sprite();

    void setPosition(int px, int py) { x = px; y = py; }
    void setScale(int s) { scale = (s <= 0 ? 1 : s); }
    void setFlipX(bool flip) { flipX = flip; }

    // Add animations by file path
    void addAnimation(const std::string& name,
                      const std::string& pngPath,
                      int frameW,
                      int frameH,
                      int framesPerStep,
                      int frameCount = -1,
                      bool looping = true);

    void setBaseAnimation(const std::string& name, bool restart = false);
    void playAction(const std::string& name, bool restart = true);
    void forceAnimation(const std::string& name, bool restart = true);
    const std::string& currentAnimationName() const { return currentName; }
    bool isActionPlaying() const { return actionPlaying; }
    void update();
    void render();

private:
    Animation* getAnim(const std::string& name);
    void switchTo(const std::string& name, bool restart, bool immediate);

private:
    SDL_Renderer* renderer = nullptr;
    int x = 0;
    int y = 0;
    int scale = 1;
    bool flipX = false;
    std::unordered_map<std::string, std::unique_ptr<Animation>> anims;
    Animation* current = nullptr;
    std::string currentName;
    std::string baseName;
    bool actionPlaying = false;
};
