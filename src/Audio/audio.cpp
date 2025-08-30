#include "audio.h"
#include <cmath>
#include <stdexcept>

// Callback que gera a onda sonora
static void audioCallback(void* userdata, Uint8* stream, int len) {
    BeepSound* beep = (BeepSound*)userdata;
    int16_t* buffer = (int16_t*)stream;
    int length = len / 2;
    // Gera onda senoidal
    for (int i = 0; i < length; i++) {
        double sample = sin(2.0 * M_PI * beep->phase) * 3000;
        buffer[i] = (int16_t)sample;
        // Avança a fase
        beep->phase += beep->freq / beep->sampleRate;
        if (beep->phase >= 1.0) beep->phase -= 1.0;
    }
}

Audio::Audio() {
    SDL_zero(want);
    SDL_zero(have);

    want.freq = beep.sampleRate;
    want.format = AUDIO_S16SYS;
    want.channels = 1;
    want.samples = 512;
    want.callback = audioCallback;
    want.userdata = &beep;
}

Audio::~Audio() {
    SDL_CloseAudioDevice(dev);
}

void Audio::init(){
    dev = SDL_OpenAudioDevice(nullptr, 0, &want, &have, 0);
    if (dev == 0) {
        throw std::runtime_error(SDL_GetError());
    }
    SDL_PauseAudioDevice(dev, 1);
}

void Audio::play() {
    SDL_PauseAudioDevice(dev, 0);
}

void Audio::stop() {
    SDL_PauseAudioDevice(dev, 1);
}
