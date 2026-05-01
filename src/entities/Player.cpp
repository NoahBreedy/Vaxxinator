#include "entities/Player.h"
#include "graphics/Sprite.h"
#include <iostream>
#include <cmath>
#include <SDL2/SDL.h>

Player::Player(int startX, int startY, SDL_Renderer* renderer, std::string bin_path)
    : x(startX), y(startY) {
    sprite = new Sprite(renderer);
    sprite->setScale(1);
    sprite->setPosition(x, y);
    setupAnimations(renderer);

    bin_file = fopen(bin_path.c_str(), "rb");

    if(!bin_file) {
        std::cout << "Failed to load binary file: " << bin_path << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

void Player::init_player_cpu(TNY_READ_FROM_BUS_FNPTR bus_read, TNY_WRITE_TO_BUS_FNPTR bus_write, class GameState* gs) {
    bool valid = tny_init_from_file(&cpu, bin_file, bus_read, bus_write);
    if(!valid) {
        std::cout << "Failed to init teenyAT: " << std::endl;
    }
    game_state = gs;
    cpu.ex_data = this;
}

void Player::setupAnimations(SDL_Renderer* renderer) {
    const int FRAME_W = 19;
    const int FRAME_H = 29;
    
    /* idle */
    sprite->addAnimation("idle_down", "assets/sprites/adventurer/Idle/standing-0.png", 
                                      FRAME_W, FRAME_H, 5, 1, true);
    sprite->addAnimation("idle_up", "assets/sprites/adventurer/Idle/standing-4.png", 
                                    FRAME_W, FRAME_H, 5, 1, true);
    sprite->addAnimation("idle_right_up", "assets/sprites/adventurer/Idle/standing-5.png", 
                                          FRAME_W, FRAME_H, 5, 1, true);
    sprite->addAnimation("idle_right_down", "assets/sprites/adventurer/Idle/standing-7.png", 
                                            FRAME_W, FRAME_H, 5, 1, true);
    sprite->addAnimation("idle_left_up", "assets/sprites/adventurer/Idle/standing-3.png", 
                                         FRAME_W, FRAME_H, 5, 1, true);
    sprite->addAnimation("idle_left_down", "assets/sprites/adventurer/Idle/standing-1.png", 
                                           FRAME_W, FRAME_H, 5, 1, true);

    /* walk */
    sprite->addAnimation("walk_down", "assets/sprites/adventurer/Walk/running_0.png", 
                                      FRAME_W, FRAME_H, 4, 4, true);
    sprite->addAnimation("walk_up", "assets/sprites/adventurer/Walk/running_4.png", 
                                    FRAME_W, FRAME_H, 4, 4, true);
    sprite->addAnimation("walk_right_up", "assets/sprites/adventurer/Walk/running_5.png", 
                                          FRAME_W, FRAME_H, 4, 4, true);
    sprite->addAnimation("walk_right_down", "assets/sprites/adventurer/Walk/running_7.png", 
                                            FRAME_W, FRAME_H, 4, 4, true);
    sprite->addAnimation("walk_left_up", "assets/sprites/adventurer/Walk/running_3.png", 
                                         FRAME_W, FRAME_H, 4, 4, true);
    sprite->addAnimation("walk_left_down", "assets/sprites/adventurer/Walk/running_1.png", 
                                           FRAME_W, FRAME_H, 4, 4, true);

    /* base */
    sprite->setBaseAnimation("idle_down", true);
}

void Player::update() {
    tny_clock(&cpu); // clock the cpu

    updateMovement();
    updateAnimation();
    
    x += velocityX;
    y += velocityY;
    
    sprite->setPosition(x, y);
    sprite->update();
}

void Player::updateMovement() {
    float newVelX = 0.0f;
    float newVelY = 0.0f;
    
    if(is_moving) {
        state = PlayerState::WALKING;
        switch (moving_direction) {
            case PlayerDirection::UP:
                newVelY -= MOVE_SPEED;
                break;
            case PlayerDirection::DOWN:
                newVelY += MOVE_SPEED;
                break;
            case PlayerDirection::LEFT:
                newVelX -= MOVE_SPEED;
                break;
            case PlayerDirection::RIGHT:
                newVelX += MOVE_SPEED;
                break;
            case PlayerDirection::UP_LEFT:
                newVelY -= MOVE_SPEED;
                newVelX -= MOVE_SPEED;
                break;
            case PlayerDirection::UP_RIGHT:
                newVelY -= MOVE_SPEED;
                newVelX += MOVE_SPEED;
                break;
            case PlayerDirection::DOWN_LEFT:
                newVelY += MOVE_SPEED;
                newVelX -= MOVE_SPEED;
                break;
            case PlayerDirection::DOWN_RIGHT:
                newVelY += MOVE_SPEED;
                newVelX += MOVE_SPEED;
                break;
            default:
                std::cout << "FATAL ERROR COMPUTER VIRUS!!!" << std::endl;
        }
    }else {
        state = PlayerState::IDLE;
    }
 
    velocityX = newVelX;
    velocityY = newVelY;

}

void Player::updateAnimation() {
    std::string baseName;
    
    if (state == PlayerState::WALKING) {
        baseName = "walk";
    } else {
        baseName = "idle";
    }
    
    std::string animName = getAnimationName(baseName, shooting_direction);
    if (sprite->currentAnimationName() != animName && !sprite->isActionPlaying()) {
        sprite->setBaseAnimation(animName, false);
    }
}

void Player::setShooting(PlayerDirection dir) {
    if((tny_uword)dir == 0){
        is_shooting = false;
        return;
    }
    shooting_direction = dir;
    is_shooting = true;
}

void Player::setMoving(PlayerDirection dir) {
    if((tny_uword)dir == 0){
        is_moving = false;
        return;
    }
    moving_direction = dir;
    is_moving = true;
}

PlayerDirection Player::getDirectionFromVelocity() const {
    
    float absX = std::abs(velocityX);
    float absY = std::abs(velocityY);
    
    if (absX > absY) {
        if (velocityX > 0) {
            if (velocityY < -0.5f) return PlayerDirection::UP_RIGHT;
            if (velocityY > 0.5f) return PlayerDirection::DOWN_RIGHT;
            return PlayerDirection::RIGHT;
        } else {
            if (velocityY < -0.5f) return PlayerDirection::UP_LEFT;
            if (velocityY > 0.5f) return PlayerDirection::DOWN_LEFT;
            return PlayerDirection::LEFT;
        }
    } 
    else {
        if (velocityY < 0) {
            if (velocityX < -0.5f) return PlayerDirection::UP_LEFT;
            if (velocityX > 0.5f) return PlayerDirection::UP_RIGHT;
            return PlayerDirection::UP;
        } else {
            if (velocityX < -0.5f) return PlayerDirection::DOWN_LEFT;
            if (velocityX > 0.5f) return PlayerDirection::DOWN_RIGHT;
            return PlayerDirection::DOWN;
        }
    }
}

std::string Player::getAnimationName(const std::string& base, PlayerDirection dir) const {
    switch (dir) {
        case PlayerDirection::UP:
            return base + "_up";
        case PlayerDirection::DOWN:
            return base + "_down";
        case PlayerDirection::LEFT:
            return base + "_left_down";  
        case PlayerDirection::RIGHT:
            return base + "_right_down";  
        case PlayerDirection::UP_LEFT:
            return base + "_left_up";
        case PlayerDirection::UP_RIGHT:
            return base + "_right_up";
        case PlayerDirection::DOWN_LEFT:
            return base + "_left_down";
        case PlayerDirection::DOWN_RIGHT:
            return base + "_right_down";
        default:
            return base + "_down";
    }
}

void Player::setPosition(int px, int py) {
    x = px;
    y = py;
    sprite->setPosition(x, y);
}

void Player::wrapPosition(int canvasWidth, int canvasHeight) {
    // Wrap X coordinate
    if (x < 0) {
        x = canvasWidth + (x % canvasWidth);
    } else if (x >= canvasWidth) {
        x = x % canvasWidth;
    }
    
    // Wrap Y coordinate
    if (y < 0) {
        y = canvasHeight + (y % canvasHeight);
    } else if (y >= canvasHeight) {
        y = y % canvasHeight;
    }
    
    sprite->setPosition(x, y);
}

void Player::getBulletSpawnPos(int& spawnX, int& spawnY) const {
    // Get the center of the sprite
    int frameWidth = sprite->getFrameWidth();
    int frameHeight = sprite->getFrameHeight();
    int scale = sprite->getScale();
    
    spawnX = x + (frameWidth * scale) / 2;
    spawnY = y + (frameHeight * scale) / 2;
}

void Player::render() {
    sprite->render();
}

SDL_Rect Player::getHitbox() const {
    int frameWidth = sprite->getFrameWidth();
    int frameHeight = sprite->getFrameHeight();
    int scale = sprite->getScale();
    return {x, y, frameWidth * scale, frameHeight * scale};
}
