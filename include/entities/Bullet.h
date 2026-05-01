#ifndef __BULLET__
#define __BULLET__

#include <SDL2/SDL.h>
#include <cmath>

class Player;

class Bullet {
public:
    Bullet(float startX, float startY, float dirX, float dirY, float speed = 5.0f);
    
    void update();
    void render(SDL_Renderer* renderer);
    
    float getX() const { return x; }
    float getY() const { return y; }
    SDL_Rect getHitbox() const;
    
    bool isOutOfBounds(int canvasWidth, int canvasHeight) const;
    
    void setSpawningPlayer(Player* player) { spawning_player = player; }
    Player* getSpawningPlayer() const { return spawning_player; }

private:
    float x;
    float y;
    float velocityX;
    float velocityY;
    Player* spawning_player = nullptr;
    static constexpr float BULLET_LENGTH = 10.0f;
    static constexpr uint8_t LINE_WIDTH = 2;
};

#endif /* __BULLET__ */
