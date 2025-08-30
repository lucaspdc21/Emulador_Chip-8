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

void Chip8::emulateCycle() {
    // Busca o opcode (2 bytes)
    uint16_t opcode = (memory[PC] << 8) | memory[PC + 1];

    // Decodifica e executa o opcode
    switch (opcode & 0xF000) {
        case 0x0000:
            switch (opcode & 0x00FF) {
                case 0x00E0: // 00E0: Limpa a tela
                    GrapBuffer.fill(0);
                    drawFlag = true;
                    PC += 2;
                    break;
                default:
                    break;
            }
            break;

        case 0x1000: // 1NNN: Pula para o endereço NNN
            PC = opcode & 0x0FFF;
            break;

        // 6XNN: O registrador Vx recebe o valor de NN
        case 0x6000:
            V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
            PC += 2;
            break;

        // 7XNN: Adiciona NN a Vx
        case 0x7000:
            V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
            PC += 2;
            break;

        // ANNN: Armazena o endereço NNN no registrador I
        case 0xA000:
            I = opcode & 0x0FFF;
            PC += 2;
            break;

        // DXYN: Desenha um sprite na tela
        case 0xD000: {
            unsigned short x = V[(opcode & 0x0F00) >> 8];
            unsigned short y = V[(opcode & 0x00F0) >> 4];
            unsigned short height = opcode & 0x000F;
            unsigned short pixel;

            V[0xF] = 0;
            for (int yline = 0; yline < height; yline++) {
                pixel = memory[I + yline];
                for (int xline = 0; xline < 8; xline++) {
                    if ((pixel & (0x80 >> xline)) != 0) {
                        if (GrapBuffer[(x + xline + ((y + yline) * 64))] == 1) {
                            V[0xF] = 1;
                        }
                        GrapBuffer[x + xline + ((y + yline) * 64)] ^= 1;
                    }
                }
            }

            drawFlag = true;
            PC += 2;
            break;
        }

        default:
            cerr << "Opcode desconhecido: " << hex << opcode << endl;
            PC += 2;
            break;
    }
}
