#include <SDL2/SDL.h>
#include <iostream>
#include "Core/Chip8.h"

using namespace std;

int main(int argc, char* argv[]){

    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <arquivo.rom>" << endl;
        return 1;
    }

    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        cerr << "Erro ao inicializar o SDL: " << SDL_GetError() << endl;
        return 1;
    }
    // Criando uma nova janela (arg: nome, pos horiz, pos vert, largura, altura e flags)
    SDL_Window* window = SDL_CreateWindow("Emulador Chip-8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 320, SDL_WINDOW_SHOWN);
    
    // SDL_CreateWindow retorna um ponteiro nulo se houver erro
    if (!window) {
        cerr << "Erro ao criar a janela: " << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    }

        SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cerr << "Erro ao criar o renderer: " << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Instancia o Chip8 e carrega ROM
    Chip8 chip8;
    chip8.loadROM(argv[1]);

    // Inicializando o evento SDL
    SDL_Event event;
    // Variável para controlar o loop principal
    bool running = true;
    
    while (running) {
        while (SDL_PollEvent(&event)) {
            // Encerra caso o evento seja do tipo QUIT
            if (event.type == SDL_QUIT) {
                running = false;
            }
            chip8.emulateCycle();
        }
        // Se precisar redesenhar a tela
        if (chip8.drawFlag) {
            chip8.drawFlag = false;

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Preto
            SDL_RenderClear(renderer);

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Branco

            for (int y = 0; y < 32; y++) {
                for (int x = 0; x < 64; x++) {
                    if (chip8.GrapBuffer[y * 64 + x]) {
                        SDL_Rect rect;
                        rect.x = x * 10;
                        rect.y = y * 10;
                        rect.w = 10;
                        rect.h = 10;
                        SDL_RenderFillRect(renderer, &rect);
                    }
                }
            }

            SDL_RenderPresent(renderer);
        }

        SDL_Delay(5);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}