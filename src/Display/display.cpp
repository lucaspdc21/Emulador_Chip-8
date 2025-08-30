#include "display.h"
#include <stdexcept>

Display::Display(int width, int height, int scale) : scale(scale) {
    window = SDL_CreateWindow("Chip-8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width * scale, height * scale, SDL_WINDOW_SHOWN);
    if (!window) {
        throw std::runtime_error("Failed to create window");
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        throw std::runtime_error("Failed to create renderer");
    }
}

Display::~Display() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void Display::clear() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void Display::drawPixel(int x, int y) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect rect = { x * scale, y * scale, scale, scale };
    SDL_RenderFillRect(renderer, &rect);
}

void Display::present() {
    SDL_RenderPresent(renderer);
}