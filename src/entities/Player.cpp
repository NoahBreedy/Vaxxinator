#include "entities/Player.h"
#include "graphics/Sprite.h"
#include <iostream>
#include <cmath>
#include <SDL2/SDL.h>

Player::Player(int startX, int startY, SDL_Renderer* renderer)
    : x(startX), y(startY) {
    sprite = std::make_unique<Sprite>(renderer);
    sprite->setScale(2);
    sprite->setPosition(x, y);
    
    setupAnimations(renderer);
}

void Player::setupAnimations(SDL_Renderer* renderer) {
    const int FRAME_W = 48;
    const int FRAME_H = 64;
    
    /* idle */
    sprite->addAnimation("idle_down", "assets/sprites/adventurer/Idle/idle_down.png", 
                                      FRAME_W, FRAME_H, 5, 4, true);
    sprite->addAnimation("idle_up", "assets/sprites/adventurer/Idle/idle_up.png", 
                                    FRAME_W, FRAME_H, 5, 4, true);
    sprite->addAnimation("idle_right_up", "assets/sprites/adventurer/Idle/idle_right_up.png", 
                                          FRAME_W, FRAME_H, 5, 4, true);
    sprite->addAnimation("idle_right_down", "assets/sprites/adventurer/Idle/idle_right_down.png", 
                                            FRAME_W, FRAME_H, 5, 4, true);
    sprite->addAnimation("idle_left_up", "assets/sprites/adventurer/Idle/idle_left_up.png", 
                                         FRAME_W, FRAME_H, 5, 4, true);
    sprite->addAnimation("idle_left_down", "assets/sprites/adventurer/Idle/idle_left_down.png", 
                                           FRAME_W, FRAME_H, 5, 4, true);

    /* walk */
    sprite->addAnimation("walk_down", "assets/sprites/adventurer/Walk/walk_down.png", 
                                      FRAME_W, FRAME_H, 5, 8, true);
    sprite->addAnimation("walk_up", "assets/sprites/adventurer/Walk/walk_up.png", 
                                    FRAME_W, FRAME_H, 5, 8, true);
    sprite->addAnimation("walk_right_up", "assets/sprites/adventurer/Walk/walk_right_up.png", 
                                          FRAME_W, FRAME_H, 5, 8, true);
    sprite->addAnimation("walk_right_down", "assets/sprites/adventurer/Walk/walk_right_down.png", 
                                            FRAME_W, FRAME_H, 5, 8, true);
    sprite->addAnimation("walk_left_up", "assets/sprites/adventurer/Walk/walk_left_up.png", 
                                         FRAME_W, FRAME_H, 5, 8, true);
    sprite->addAnimation("walk_left_down", "assets/sprites/adventurer/Walk/walk_left_down.png", 
                                           FRAME_W, FRAME_H, 5, 8, true);

    /* base */
    sprite->setBaseAnimation("idle_down", true);
}

void Player::update(const bool* keys) {
    updateMovement(keys);
    updateAnimation();
    
    x += velocityX;
    y += velocityY;
    
    sprite->setPosition(x, y);
    sprite->update();
}

void Player::updateMovement(const bool* keys) {
    float newVelX = 0.0f;
    float newVelY = 0.0f;
    
    if (keys[SDL_SCANCODE_UP]) {
        newVelY -= MOVE_SPEED;
    }
    if (keys[SDL_SCANCODE_DOWN]) {
        newVelY += MOVE_SPEED;
    }
    if (keys[SDL_SCANCODE_LEFT]) {
        newVelX -= MOVE_SPEED;
    }
    if (keys[SDL_SCANCODE_RIGHT]) {
        newVelX += MOVE_SPEED;
    }
    
    velocityX = newVelX;
    velocityY = newVelY;
    
    if (std::abs(velocityX) > 0.1f || std::abs(velocityY) > 0.1f) {
        lastDirection = getDirectionFromVelocity();
        direction = lastDirection;
        state = PlayerState::WALKING;
    } else {
        state = PlayerState::IDLE;
        direction = lastDirection;
    }
}

void Player::updateAnimation() {
    std::string baseName;
    
    if (state == PlayerState::WALKING) {
        baseName = "walk";
    } else {
        baseName = "idle";
    }
    
    std::string animName = getAnimationName(baseName, direction);
    if (sprite->currentAnimationName() != animName && !sprite->isActionPlaying()) {
        sprite->setBaseAnimation(animName, false);
    }
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

void Player::render() {
    sprite->render();
}
