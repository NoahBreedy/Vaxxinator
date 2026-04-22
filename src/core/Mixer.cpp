#include "core/Mixer.h"
#include <iostream>

Mixer::Mixer()
    : current_sound(nullptr),
      current_music(nullptr),
      initialized(false) {
}

Mixer::~Mixer() {
    shutdown();
}

bool Mixer::init() {
    if (initialized)
        return true;

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "Mix_OpenAudio failed: " << Mix_GetError() << std::endl;
        return false;
    }

    Mix_AllocateChannels(16);

    initialized = true;
    return true;
}

void Mixer::shutdown() {
    if (!initialized)
        return;

    stop_audio();
    stop_music();

    Mix_CloseAudio();

    initialized = false;
}

bool Mixer::play_audio(const std::string& path, int loops) {
    if (!initialized)
        return false;

    if (current_sound) {
        Mix_FreeChunk(current_sound);
        current_sound = nullptr;
    }

    current_sound = Mix_LoadWAV(path.c_str());

    if (!current_sound) {
        std::cout << "Failed loading sound: " << Mix_GetError() << std::endl;
        return false;
    }

    if (Mix_PlayChannel(-1, current_sound, loops) == -1) {
        std::cout << "Failed playing sound: " << Mix_GetError() << std::endl;
        return false;
    }

    return true;
}

void Mixer::stop_audio() {
    Mix_HaltChannel(-1);

    if (current_sound) {
        Mix_FreeChunk(current_sound);
        current_sound = nullptr;
    }
}

bool Mixer::play_music(const std::string& path, int loops) {
    if (!initialized)
        return false;

    if (current_music) {
        Mix_HaltMusic();
        Mix_FreeMusic(current_music);
        current_music = nullptr;
    }

    current_music = Mix_LoadMUS(path.c_str());

    if (!current_music) {
        std::cout << "Failed loading music: " << Mix_GetError() << std::endl;
        return false;
    }

    if (Mix_PlayMusic(current_music, loops) == -1) {
        std::cout << "Failed playing music: " << Mix_GetError() << std::endl;
        return false;
    }

    return true;
}

void Mixer::stop_music() {
    Mix_HaltMusic();

    if (current_music) {
        Mix_FreeMusic(current_music);
        current_music = nullptr;
    }
}

void Mixer::pause_music() {
    Mix_PauseMusic();
}

void Mixer::resume_music() {
    Mix_ResumeMusic();
}

void Mixer::set_sound_volume(int volume) {
    Mix_Volume(-1, volume);
}

void Mixer::set_music_volume(int volume) {
    Mix_VolumeMusic(volume);
}