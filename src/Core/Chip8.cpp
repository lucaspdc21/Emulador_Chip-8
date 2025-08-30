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

void Chip8::loadROM(const char *filename) {
    ifstream romFile(filename, ios::binary | ios::ate); // Abre como binário e vai para o final
    if (!romFile) {
        cerr << "Erro ao abrir o arquivo ROM: " << filename << endl;
        return;
    }

    streamsize size = romFile.tellg(); // Captura o tamanho do arquivo
    romFile.seekg(0, ios::beg);
    // Retorna ao inicio do arquivo para leitura
    romFile.seekg(0, std::ios::beg);
    if (size > MEMORY_SIZE - 0x200) {
        cerr << "Arquivo ROM muito grande: " << filename << endl;
        return;
    }

    romFile.read(reinterpret_cast<char*>(&memory[0x200]), size);
    if (!romFile) {
        std::cerr << "Erro ao ler a ROM." << std::endl;
        return;
    }
    romFile.close();
}