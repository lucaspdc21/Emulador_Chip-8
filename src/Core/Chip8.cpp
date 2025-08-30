#include "Chip8.h"
#include <fstream>
#include <iostream>

using namespace std;

Chip8::Chip8() {
    // Estado inicial, zerando memória e registradores
    memory.fill(0);
    V.fill(0);
    stack.fill(0);
    keypad.fill(0);
    I = 0;
    PC = 0x200; // A especificação manda executar a partir do endereço 0x200
    DT = 0;
    ST = 0;
    SP = 0;
    drawFlag = false;
}

Chip8::~Chip8() {}