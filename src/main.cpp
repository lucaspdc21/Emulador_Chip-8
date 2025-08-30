#include <SDL2/SDL.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]){
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
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}