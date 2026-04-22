#include <ctime>
#include <iostream>
#include "states/GameState.h"

#define STATE_NAME "GameState"
GameState::GameState(StateMachine* machine): State(STATE_NAME, machine)
{
    /* Setup texture for pixel modification */
    format = 0;
    pitch = 0;
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    background_texture = SDL_CreateTexture(
        state_machine->renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        CANVAS_WIDTH,
        CANVAS_HEIGHT
    );
    SDL_QueryTexture(background_texture, &format , nullptr, nullptr, nullptr);
    SDL_PixelFormat *pixel_format = SDL_AllocFormat(format);

    /* For now I make the color map hold purple but we can randomize it... */
    for(int i = 0; i < 256; i++){
        color_map[i] = SDL_MapRGB(pixel_format,i,0,i);
    }
    
    pixels = nullptr;
    strand_size = 32;

    frame_count = 0;
}

void GameState::render() {
    SDL_SetRenderDrawColor(state_machine->renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(state_machine->renderer);

    /* Draw simplex noise background
     * to describe this algorithm for future Noah it basically 
     * calulates the simplex value based on the x & y coordinate of 
     * the texture from 2 different generators along with our frame 
     * count to get a sort of movement "animation. It then combines 
     * them at different proportions 1/15 and 2/15 to get ourfinal 
     * simplex value. We then just use this value to get an index into 
     * a colormap and the result is cool looking moving "veins"
     */
    SDL_LockTexture(background_texture, nullptr, &pixels, &pitch);
    uint32_t* target_pixels = (uint32_t*)pixels;
    int pixels_per_row = pitch / sizeof(uint32_t); 
    for (int y = 0; y < CANVAS_HEIGHT; ++y) {
            for (int x = 0; x < CANVAS_WIDTH; ++x) {
                double v = open_simplex_noise3(noise_ctx0 , (double)x / strand_size  /  1.0,(double) y  / strand_size / 1.0 , (double)frame_count   /   (8  * MOVEMENT_SCALER));
                double v2 = open_simplex_noise3(noise_ctx1, (double)x / strand_size  /  2.0,(double) y  / strand_size / 2.0 , (double)frame_count   /   (16 * MOVEMENT_SCALER));
                
                v = (v *  1.0  / 15.0) + (v2 * 2.0 / 15.0);

                uint8_t color_index  = 256.0 * std::abs(v);
                
                target_pixels[y * pixels_per_row + x] = color_map[color_index];
            }
    }
    SDL_UnlockTexture(background_texture); // Let the gpu take over...

    /* Blit the texture to the renderer */
    SDL_RenderCopy(state_machine->renderer, background_texture, nullptr, nullptr);

    for(BloodCell& cell : cells) {
        cell.render(state_machine->renderer);
    }
    
    for(Player& player : players) {
        player.render();
    }
    
    for(Bullet& bullet : bullets) {
        bullet.render(state_machine->renderer);
    }

}

void GameState::update() {
    if (state_machine->input_buffer.has_key()) {
        SDL_Keycode key = state_machine->input_buffer.pop_key();

        if (key == SDLK_SPACE) {
            state_machine->transition("MainMenu");
        }
    }

    frame_count++;
    
    for(int i = 0; i < cells.size(); i++) {
        cells[i].update();
    }
    
    for(int i = 0; i < players.size(); i++) {
        players[i].update();
    }
    
    // Wrap player positions around screen
    for(int i = 0; i < players.size(); i++) {
        players[i].wrapPosition(CANVAS_WIDTH, CANVAS_HEIGHT);
    }
    
    // Spawn bullets from shooting players every BULLET_SPAWN_RATE cycles
    if (frame_count % BULLET_SPAWN_RATE == 0) {
        for(Player& player : players) {
            if(player.isShooting()) {

                state_machine->audio_mixer.play_audio("assets/audio/laser.wav");

                PlayerDirection shootDir = player.getShooting();
                
                float dirX = 0.0f, dirY = 0.0f;
                switch(shootDir) {
                    case PlayerDirection::UP:
                        dirX = 0.0f;
                        dirY = -1.0f;
                        break;
                    case PlayerDirection::DOWN:
                        dirX = 0.0f;
                        dirY = 1.0f;
                        break;
                    case PlayerDirection::LEFT:
                        dirX = -1.0f;
                        dirY = 0.0f;
                        break;
                    case PlayerDirection::RIGHT:
                        dirX = 1.0f;
                        dirY = 0.0f;
                        break;
                    case PlayerDirection::UP_LEFT:
                        dirX = -0.707f;
                        dirY = -0.707f;
                        break;
                    case PlayerDirection::UP_RIGHT:
                        dirX = 0.707f;
                        dirY = -0.707f;
                        break;
                    case PlayerDirection::DOWN_LEFT:
                        dirX = -0.707f;
                        dirY = 0.707f;
                        break;
                    case PlayerDirection::DOWN_RIGHT:
                        dirX = 0.707f;
                        dirY = 0.707f;
                        break;
                    default:
                        dirX = 0.0f;
                        dirY = 1.0f;
                }
                
                int spawnX, spawnY;
                player.getBulletSpawnPos(spawnX, spawnY);
                bullets.push_back(Bullet(spawnX, spawnY, dirX, dirY));
            }
        }
    }
    
    // Update all bullets
    for(int i = 0; i < bullets.size(); i++) {
        bullets[i].update();
    }
    
    // Remove out of bounds bullets
    for(int i = bullets.size() - 1; i >= 0; i--) {
        if(bullets[i].isOutOfBounds(CANVAS_WIDTH, CANVAS_HEIGHT)) {
            bullets.erase(bullets.begin() + i);
        }
    }

    SDL_RenderPresent(state_machine->renderer);
    SDL_Delay(10);
}

void GameState::enter() {
    std::srand(std::time(nullptr));
    open_simplex_noise(std::rand(),&noise_ctx0);
    open_simplex_noise(std::rand(),&noise_ctx1);
    
    /* create vector of players with the associated binary files 
    *  and then init their cpu
    */
    players.reserve(4); // reserve space for 4 players to avoid reallocations
    int index = 0;
    for(int i = 0; i < 4; i++) { 
        if(state_machine->syringe_paths[i] != ""){
            players.push_back(Player(std::rand() % CANVAS_WIDTH, std::rand() % CANVAS_HEIGHT, state_machine->renderer, state_machine->syringe_paths[i]));
            players[index].init_player_cpu(GameState::bus_read, GameState::bus_write, this);
            index++;
        }
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    background_texture = SDL_CreateTexture(
        state_machine->renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        CANVAS_WIDTH,
        CANVAS_HEIGHT
    );

    SDL_RenderSetLogicalSize(state_machine->renderer, CANVAS_WIDTH, CANVAS_HEIGHT);
    SDL_RenderSetIntegerScale(state_machine->renderer, SDL_TRUE);
    
    frame_count = 0;

    cells.push_back(BloodCell(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, 8, true, state_machine->renderer));
    for(int i = 0; i < 4; i++) {
        cells.push_back(BloodCell((int)rand_float(8,CANVAS_WIDTH-8), (int)rand_float(8,CANVAS_HEIGHT-8), 8, std::rand() % 2, state_machine->renderer));
    }
    
}

void GameState::exit() {
    SDL_RenderSetLogicalSize(state_machine->renderer, state_machine->window_width, state_machine->window_height);

    open_simplex_noise_free(noise_ctx0);
    open_simplex_noise_free(noise_ctx1);
    SDL_DestroyTexture(background_texture);
    pixels = nullptr;
    state_machine->audio_mixer.stop_music();
    cells.clear();
    players.clear();
    bullets.clear();

}

/* TeenyAT Bus Read and Bus Write functions */
void GameState::bus_read(teenyat *t, tny_uword addr, tny_word *data, uint16_t *delay) {
    Player* player = (Player*)t->ex_data;
    switch(addr) {
        case SHOOT_DIR:
            data->u = (tny_uword)player->getShooting();
            break;
        case MOVE_DIR:
            data->u = (tny_uword)player->getMoving();
            break;
        case NEAREST_BLOODCELL_DIR:
            {
                // Find the index of the nearest blood cell to the player
                int playerX = player->getX();
                int playerY = player->getY();
                int nearestIndex = 0;
                float nearestDist = std::numeric_limits<float>::max();
                for(int i = 0; i < player->getGameState()->cells.size(); i++) {
                    BloodCell& cell = player->getGameState()->cells[i];
                    float dist = std::sqrt(std::pow(playerX - cell.getX(), 2) + std::pow(playerY - cell.getY(), 2));
                    if(dist < nearestDist) {
                        nearestDist = dist;
                        nearestIndex = i;
                    }
                }
                
                // Calculate direction from player to nearest cell
                BloodCell& nearestCell = player->getGameState()->cells[nearestIndex];
                float dx = nearestCell.getX() - playerX;
                float dy = nearestCell.getY() - playerY;

                // atan2 gives angle in (-π, π], divide circle into 8 equal 45° sectors
                float angle = std::atan2(dy, dx) * (180.0f / M_PI); // -180 to 180
                if (angle < 0) angle += 360.0f;                      // 0 to 360
                angle = std::fmod(angle + 270.0f, 360.0f); // rotate so 0° = DOWN

                PlayerDirection direction;
                if      (angle <  22.5f || angle >= 337.5f) direction = PlayerDirection::DOWN;
                else if (angle <  67.5f)                    direction = PlayerDirection::DOWN_LEFT;
                else if (angle < 112.5f)                    direction = PlayerDirection::LEFT;
                else if (angle < 157.5f)                    direction = PlayerDirection::UP_LEFT;
                else if (angle < 202.5f)                    direction = PlayerDirection::UP;
                else if (angle < 247.5f)                    direction = PlayerDirection::UP_RIGHT;
                else if (angle < 292.5f)                    direction = PlayerDirection::RIGHT;
                else                                        direction = PlayerDirection::DOWN_RIGHT;

                data->u = (tny_uword)direction;
                break;
            }
        case NEAREST_PLAYER_DIR:
            {
                if (player->getGameState()->players.size() <= 1) {
                    data->u = (tny_uword)PlayerDirection::NONE; // No other players
                    break;
                }

                // Find the index of the nearest player to the player
                int playerX = player->getX();
                int playerY = player->getY();
                int nearestIndex = 0;
                float nearestDist = std::numeric_limits<float>::max();
                for(int i = 0; i < player->getGameState()->players.size(); i++) {
                    Player& otherPlayer = player->getGameState()->players[i];
                    if(&otherPlayer == player) continue; // skip self
                    float dist = std::sqrt(std::pow(playerX - otherPlayer.getX(), 2) + std::pow(playerY - otherPlayer.getY(), 2));
                    if(dist < nearestDist) {
                        nearestDist = dist;
                        nearestIndex = i;
                    }
                }
                
                // Calculate direction from player to nearest other player
                Player& nearestPlayer = player->getGameState()->players[nearestIndex];
                float dx = nearestPlayer.getX() - playerX;
                float dy = nearestPlayer.getY() - playerY;

                // atan2 gives angle in (-π, π], divide circle into 8 equal 45° sectors
                float angle = std::atan2(dy, dx) * (180.0f / M_PI); // -180 to 180
                if (angle < 0) angle += 360.0f;                      // 0 to 360
                angle = std::fmod(angle + 270.0f, 360.0f); // rotate so 0° = DOWN

                PlayerDirection direction;
                if      (angle <  22.5f || angle >= 337.5f) direction = PlayerDirection::DOWN;
                else if (angle <  67.5f)                    direction = PlayerDirection::DOWN_LEFT;
                else if (angle < 112.5f)                    direction = PlayerDirection::LEFT;
                else if (angle < 157.5f)                    direction = PlayerDirection::UP_LEFT;
                else if (angle < 202.5f)                    direction = PlayerDirection::UP;
                else if (angle < 247.5f)                    direction = PlayerDirection::UP_RIGHT;
                else if (angle < 292.5f)                    direction = PlayerDirection::RIGHT;
                else                                        direction = PlayerDirection::DOWN_RIGHT;

                data->u = (tny_uword)direction;
                break;
            } 
        default:
            break;
    }
}

void GameState::bus_write(teenyat *t, tny_uword addr, tny_word data, uint16_t *delay) {
    Player* player = (Player*)t->ex_data;
    switch(addr) {
        case SHOOT_DIR:
            player->setShooting((PlayerDirection)data.u);
            break;
        case MOVE_DIR:
            player->setMoving((PlayerDirection)data.u);
            break;
        default:
            break;
    }
}