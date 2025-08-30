#pragma once
#include <SDL2/SDL.h>
#include <array>

class Keyboard {
public:
    // Atualiza o estado do teclado
    static void update(std::array<uint8_t, 16>& keypad, const SDL_Event& event);

private:
    // Mapeamento do teclado físico para o teclado Chip-8
    static int mapKey(SDL_Keycode key);
};
