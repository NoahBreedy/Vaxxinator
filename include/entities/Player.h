#ifndef __PLAYER__
#define __PLAYER__

#include <SDL2/SDL.h>
#include <memory>
#include <string>
#include "core/teenyat.h"

class Sprite;
class GameState;

enum class PlayerDirection {
    NONE,
    DOWN,
    DOWN_LEFT,
    LEFT,
    UP_LEFT,
    UP,
    UP_RIGHT,
    RIGHT,
    DOWN_RIGHT,
};

enum class PlayerState {
    IDLE,
    WALKING
};

class Player {
public:
    explicit Player(int startX, int startY, SDL_Renderer* renderer, std::string bin_path);
    ~Player() = default;

    void update();
    void render();

    void setPosition(int px, int py);

    PlayerDirection getShooting() const { return shooting_direction; }
    bool isShooting() const { return is_shooting; }
    void setShooting(PlayerDirection dir);

    PlayerDirection getMoving() const { return moving_direction; }
    void setMoving(PlayerDirection dir);

    void wrapPosition(int canvasWidth, int canvasHeight);

    int getX() const { return x; }
    int getY() const { return y; }

    void init_player_cpu(TNY_READ_FROM_BUS_FNPTR bus_read, TNY_WRITE_TO_BUS_FNPTR bus_write, class GameState* game_state);
    
    void getBulletSpawnPos(int& spawnX, int& spawnY) const;
    SDL_Rect getHitbox() const;

    class GameState* getGameState() const { return game_state; }
    
    int getHealth() const { return health; }
    void setHealth(int h) { health = h; }
    void takeDamage(int damage) { health -= damage; }
    bool isAlive() const { return health > 0; }

    int getScore() const { return player_score; }
    void addScore(uint64_t points) { player_score += points; }
private:
    Sprite* sprite;
    
    int x;
    int y;
    int health = 100;
    static constexpr float MOVE_SPEED = 3.0f;
    float velocityX = 0.0f;
    float velocityY = 0.0f;
    uint64_t player_score = 0;

    bool is_shooting = false;
    bool is_moving = false;

    FILE* bin_file;

    teenyat cpu;
    class GameState* game_state = nullptr;

    PlayerState state = PlayerState::IDLE;
    PlayerDirection shooting_direction = PlayerDirection::DOWN;
    PlayerDirection moving_direction   = PlayerDirection::DOWN;

    void setupAnimations(SDL_Renderer* renderer);
    
    void updateMovement();
    void updateAnimation();

    std::string getAnimationName(const std::string& base, PlayerDirection dir) const;
    PlayerDirection getDirectionFromVelocity() const;
};

#endif /* __PLAYER__ */
