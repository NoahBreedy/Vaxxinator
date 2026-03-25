#ifndef __PLAYER__
#define __PLAYER__

#include <SDL2/SDL.h>
#include <memory>
#include <string>
#include "core/teenyat.h"

class Sprite;

enum class PlayerDirection {
    DOWN = 0,
    DOWN_LEFT = 1,
    LEFT = 2,
    UP_LEFT = 3,
    UP = 4,
    UP_RIGHT = 5,
    RIGHT = 6,
    DOWN_RIGHT = 7,
    NONE = 8
};

enum class PlayerState {
    IDLE,
    WALKING
};

class Player {
public:
    explicit Player(int startX, int startY, SDL_Renderer* renderer, std::string bin_path);
    ~Player() = default;

    void update(const bool* keys);
    void render();

    void setPosition(int px, int py);
    int getX() const { return x; }
    int getY() const { return y; }

    void init_player_cpu(TNY_READ_FROM_BUS_FNPTR bus_read, TNY_WRITE_TO_BUS_FNPTR bus_write);

private:
    Sprite* sprite;
    
    int x;
    int y;
    static constexpr float MOVE_SPEED = 3.0f;
    float velocityX = 0.0f;
    float velocityY = 0.0f;

    FILE* bin_file;

    teenyat cpu;

    PlayerState state = PlayerState::IDLE;
    PlayerDirection direction = PlayerDirection::DOWN;
    PlayerDirection lastDirection = PlayerDirection::DOWN;

    void setupAnimations(SDL_Renderer* renderer);
    
    void updateMovement(const bool* keys);
    void updateAnimation();

    std::string getAnimationName(const std::string& base, PlayerDirection dir) const;
    PlayerDirection getDirectionFromVelocity() const;
};

#endif /* __PLAYER__ */
