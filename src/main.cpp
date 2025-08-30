#include <SDL2/SDL.h>
#include <iostream>
#include "Core/Chip8.h"
#include "Display/display.h"
#include "Audio/audio.h"
#include "Loader/loader.h"

using namespace std;

int main(int argc, char* argv[]){

    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <arquivo.rom>" << endl;
        return 1;
    }

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0) {
        cerr << "Erro ao inicializar SDL: " << SDL_GetError() << endl;
        return 1;
    }

    // CHip8 usa um display 64x32, vamos gerar uma window de 640x320
    Display display(64, 32, 10);
    // Instanciando classe de áudio
    Audio audio;
    audio.init();

    // Instancia o Chip8 e carrega ROM
    Chip8 chip8;
    chip8.loadROM(argv[1]);

    // Inicializando o evento SDL
    SDL_Event event;
    // Variável para controlar o loop principal
    bool running = true;

    // Marca o tempo da última atualização dos timers
    Uint32 lastTimerUpdate = SDL_GetTicks();
    
    while (running) {
        while (SDL_PollEvent(&event)) {
            // Encerra caso o evento seja do tipo QUIT
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
        chip8.emulateCycle();
        // Verifica o estado de drawFlag para redesenhar a tela
        if (chip8.drawFlag) {
            chip8.drawFlag = false;
            display.clear();
            for (int y = 0; y < 32; y++) {
                for (int x = 0; x < 64; x++) {
                    if (chip8.GrapBuffer[y * 64 + x]) {
                        display.drawPixel(x, y);
                    }
                }
            }
            display.present();
        }
        Uint32 now = SDL_GetTicks();
        if (now - lastTimerUpdate >= 1000 / 60) {
            chip8.updateTimers();
            if(chip8.ST > 0) {
                audio.play();
            } else {
                audio.stop();
            }
            lastTimerUpdate = now;
        }

        SDL_Delay(5);
    }
    SDL_Quit();
    return 0;
}