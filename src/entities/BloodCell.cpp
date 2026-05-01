#include "entities/BloodCell.h"
#include "graphics/Sprite.h"
#include "core/StateMachine.h"

BloodCell::BloodCell(uint16_t _x, uint16_t _y, uint16_t _radius, bool is_white, SDL_Renderer *renderer) {
    
    for(int i = 0; i < NUM_BALLS; i++) {
        positions[i].x = _x;
        positions[i].y = _y;
        velocities[i].x =  rand_float(-0.1,0.1);
        velocities[i].y =  rand_float(-0.1,0.1);
    }

    /* This is the speed of the cell */
    velocities[0].x =  rand_float(-0.3,0.3);
    velocities[0].y =  rand_float(-0.3,0.3);

    radius     = _radius;
    cell_color = is_white ? RGBA{255,255,255,255} : RGBA{245,72,66,255};
    sprite = new Sprite(renderer);

    sprite->setScale(1);
    sprite->setPosition(positions[0].x-radius, positions[0].y-radius);

    // Idle
    sprite->addAnimation("idle", "assets/sprites/cells/cell_face_0.png",  16, 16, 8, -1, true);
    sprite->setBaseAnimation("idle", true);
}

void BloodCell::render(SDL_Renderer *renderer) {
    /* First draw every circle in black */
    for(int i = 0; i < NUM_BALLS; i++) {
        filledCircleRGBA(renderer, positions[i].x, positions[i].y, (radius), 0, 0, 0, 255);
    }

    /* Then draw all inner circles in white with (r-i)-1 */
    for(int i = 0; i < NUM_BALLS; i++) {
        filledCircleRGBA(renderer, positions[i].x, positions[i].y, (radius - i) - 1, cell_color.r, cell_color.g, cell_color.b, cell_color.a);
    }

    /* Lastly draw outer circle in white with r-1 */
    filledCircleRGBA(renderer, positions[0].x , positions[0].y, radius - 1, cell_color.r, cell_color.g, cell_color.b, cell_color.a);

    sprite->render();
}

void BloodCell::update() {
    if(rand_float(0,1) > MOVEMENT_THRESHOLD) {  
        sprite->setPosition(positions[0].x + velocities[0].x * 2 - radius, positions[0].y + velocities[0].y * 2 - radius);
        sprite->update();

        positions[0].x += velocities[0].x;
        if(positions[0].x < radius || positions[0].x >= CANVAS_WIDTH - radius) {
            velocities[0].x *= -1.0;
        }

        positions[0].y += velocities[0].y;
        if(positions[0].y < radius || positions[0].y >= CANVAS_HEIGHT - radius) {
            velocities[0].y *= -1.0;
        }

        for(int i = 1; i < NUM_BALLS; i++) {
            /* Add base velocity */
            positions[i].x += velocities[0].x;
            positions[i].y += velocities[0].y;
            
            positions[i].x = positions[i].x + velocities[i].x;
            if(distance(positions[i].x, positions[i].y, positions[0].x, positions[0].y) > 1.5) {
                velocities[i].x *= -1.0;
            }
            
            positions[i].y += velocities[i].y;
            if(distance(positions[i].x, positions[i].y, positions[0].x, positions[0].y) > 1.5) {
                velocities[i].y *= -1.0;
            }
        }

        if(rand_float(-1,1) > CHANGE_MOVEMENT_THRESHOLD) {
            velocities[0].x =  rand_float(-0.3,0.3);
            velocities[0].y =  rand_float(-0.3,0.3);
        }
    }
}

SDL_Rect BloodCell::getHitbox() const {
    int x = (int)positions[0].x;
    int y = (int)positions[0].y;
    return {x - radius, y - radius, radius * 2, radius * 2};
}