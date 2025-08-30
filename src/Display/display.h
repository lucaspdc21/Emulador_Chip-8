#pragma once
#include <SDL2/SDL.h>

class Display {
public:
    Display(int width, int height, int scale);
    ~Display();

    void clear();
    void drawPixel(int x, int y);
    void present();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    int scale;
};
