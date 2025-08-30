#ifndef CHIP8_H
#define CHIP8_H

#include <array>
#include <cstdint>
#include <string>

using namespace std;

class Chip8 {
public:
    Chip8();
    ~Chip8();

    static const int MEMORY_SIZE = 4096;

    array<uint8_t, 16> keypad;
    array<uint8_t, 64*32> GrapBuffer;

    // Indica quando devemos redesenhar a tela
    bool drawFlag; 
    void emulateCycle();
    void updateTimers();
    void loadROM(const string& romName);
    
    uint8_t DT; // Delay timer
    uint8_t ST; // Sound timer
private:
    array<uint16_t, 16> stack;

    array<uint8_t, MEMORY_SIZE> memory;
    array<uint8_t, 16> V; // Registradores V0 a VF
    
    uint16_t I; // Registrador index
    uint16_t PC; // Program counter
    uint16_t SP; // Stack pointer


};

#endif // CHIP8_H