#ifndef __PLAYER__
#define __PLAYER__

#include <SDL2/SDL.h>
#include <memory>

class Sprite;

enum class PlayerDirection {
    UP = 0,
    UP_RIGHT = 1,
    RIGHT = 2,
    DOWN_RIGHT = 3,
    DOWN = 4,
    DOWN_LEFT = 5,
    LEFT = 6,
    UP_LEFT = 7,
    NONE = 8
};

enum class PlayerState {
    IDLE,
    WALKING
};

class Player {
public:
    explicit Player(int startX, int startY, SDL_Renderer* renderer);
    ~Player() = default;

    void update(const bool* keys);
    void render();

    void setPosition(int px, int py);
    int getX() const { return x; }
    int getY() const { return y; }

private:
    std::unique_ptr<Sprite> sprite;
    
    int x;
    int y;
    static constexpr float MOVE_SPEED = 3.0f;
    float velocityX = 0.0f;
    float velocityY = 0.0f;

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
