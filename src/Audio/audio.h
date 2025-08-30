#pragma once
#include <SDL2/SDL.h>

struct BeepSound {
    double phase = 0.0;
    double freq = 440.0; // Frequência lá 
    int sampleRate = 44100;
};

class Audio {
public:
    Audio();
    ~Audio();

    void init();
    void play();
    void stop();

private:
    BeepSound beep;
    SDL_AudioDeviceID dev;
    SDL_AudioSpec want, have;
};
