#include "graphics/Sprite.h"
#include "graphics/Animation.h"
#include <iostream>

Sprite::Sprite(SDL_Renderer* renderer)
    : renderer(renderer) {}

Sprite::~Sprite() = default;

Animation* Sprite::getAnim(const std::string& name) {
    auto it = anims.find(name);
    if (it == anims.end()) return nullptr;
    return it->second.get();
}

void Sprite::addAnimation(const std::string& name,
                          const std::string& pngPath,
                          int frameW,
                          int frameH,
                          int framesPerStep,
                          int frameCount,
                          bool looping) {
    auto anim = std::make_unique<Animation>(renderer, pngPath, frameW, frameH, framesPerStep, frameCount, looping);
    if (!anim->ok()) {
        std::cerr << "Failed to add animation '" << name << "' from " << pngPath << "\n";
        return;
    }

    anims[name] = std::move(anim);

    if (!current) {
        baseName = name;
        switchTo(name, true, true);
    }
}

void Sprite::switchTo(const std::string& name, bool restart, bool immediate) {
    Animation* next = getAnim(name);
    if (!next) {
        std::cerr << "switchTo: unknown animation '" << name << "'\n";
        return;
    }

    if (name == currentName) {
        if (restart) next->reset(immediate);
        return;
    }

    current = next;
    currentName = name;

    if (restart) current->reset(immediate);
}

void Sprite::setBaseAnimation(const std::string& name, bool restart) {
    baseName = name;
    if (actionPlaying) return;
    switchTo(baseName, restart, true);
}

void Sprite::playAction(const std::string& name, bool restart) {
    Animation* a = getAnim(name);
    if (!a) {
        std::cerr << "playAction: unknown animation '" << name << "'\n";
        return;
    }

    a->setLooping(false);
    actionPlaying = true;
    switchTo(name, restart, true);
}

void Sprite::forceAnimation(const std::string& name, bool restart) {
    actionPlaying = false;
    switchTo(name, restart, true);
}

void Sprite::update() {
    if (!current) return;

    current->update();
    if (actionPlaying && current->isFinished()) {
        actionPlaying = false;

        if (!baseName.empty()) {
            switchTo(baseName, true, true);
        }
    }
}

void Sprite::render() {
    if (!current) return;

    SDL_Texture* tex = current->getTexture();
    if (!tex) return;

    SDL_Rect src = current->getSrcRect();
    SDL_Rect dst = { x, y, src.w * scale, src.h * scale };

    SDL_RendererFlip flip = flipX ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    if (SDL_RenderCopyEx(renderer, tex, &src, &dst, 0.0, nullptr, flip) != 0) {
        std::cerr << "SDL_RenderCopyEx failed: " << SDL_GetError() << "\n";
    }
}

int Sprite::getFrameWidth() const {
    if (!current) return 0;
    return current->getFrameW();
}

int Sprite::getFrameHeight() const {
    if (!current) return 0;
    return current->getFrameH();
}

