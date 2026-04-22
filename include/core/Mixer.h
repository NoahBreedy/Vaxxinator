// Mixer.h
#ifndef __MIXER__
#define __MIXER__

#include <string>
#include <SDL.h>
#include <SDL_mixer.h>

class Mixer {
public:
    Mixer();
    ~Mixer();

    bool init();
    void shutdown();

    // Sound effects (.wav)
    bool play_audio(const std::string& path, int loops = 0);
    void stop_audio();

    // Music (.mp3 / .ogg / .wav)
    bool play_music(const std::string& path, int loops = -1);
    void stop_music();

    void pause_music();
    void resume_music();

    void set_sound_volume(int volume); // 0-128
    void set_music_volume(int volume); // 0-128

private:
    Mix_Chunk* current_sound;
    Mix_Music* current_music;
    bool initialized;
};

#endif /* __MIXER__ */