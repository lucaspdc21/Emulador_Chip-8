#include "Chip8.h"
#include "../Loader/loader.h"
#include <fstream>
#include <iostream>

using namespace std;

Chip8::Chip8() {
    // Estado inicial, zerando memória e registradores
    memory.fill(0);
    V.fill(0);
    stack.fill(0);
    keypad.fill(0);
    GrapBuffer.fill(0);
    I = 0;
    PC = 0x200; // A especificação manda executar a partir do endereço 0x200
    DT = 0;
    ST = 0;
    SP = 0;
    drawFlag = false;
}

Chip8::~Chip8() {}

void Chip8::emulateCycle() {
    // Busca o opcode (2 bytes)
    uint16_t opcode = (memory[PC] << 8) | memory[PC + 1];

    // Decodifica e executa o opcode
    switch (opcode & 0xF000) {
        case 0x0000:
            switch (opcode & 0x00FF) {
                // 00E0: Limpa a tela
                case 0x00E0:
                    GrapBuffer.fill(0);
                    drawFlag = true;
                    PC += 2;
                    break;
                
                case 0x00EE: // Retorna da sub-rotina
                    --SP;
                    PC = stack[SP] + 2;
                    break;

                default:
                    break;
            }
            break;

        case 0x1000: // 1NNN: Pula para o endereço NNN
            PC = opcode & 0x0FFF;
            break;

        // 2NNN: Chama a sub-rotina no endereço NNN
        case 0x2000:
            stack[SP++] = PC;
            PC = opcode & 0x0FFF;
            break;

        // 3XNN: Pula a próxima instrução se Vx for igual a NN
        case 0x3000:
            if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) {
                PC += 4;
            } else {
                PC += 2;
            }
            break;

        // 4XNN: Pula a próxima instrução se Vx não for igual a NN
        case 0x4000:
            if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)) {
                PC += 4;
            } else {
                PC += 2;
            }
            break;
        // 5XY0: Pula a próxima instrução se Vx for igual a Vy
        case 0x5000:
            if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4]) {
                PC += 4;
            } else {
                PC += 2;
            }
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
        case 0x8000:
            switch (opcode & 0x000F) {
                // 8XY0: O registrador Vx recebe o valor de Vy
                case 0x0000:
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
                    PC += 2;
                    break;

                // 8XY1: O registrador Vx recebe o valor de Vx OR Vy
                case 0x0001:
                    V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
                    PC += 2;
                    break;

                // 8XY2: O registrador Vx recebe o valor de Vx AND Vy
                case 0x0002:
                    V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
                    PC += 2;
                    break;

                // 8XY3: O registrador Vx recebe o valor de Vx XOR Vy
                case 0x0003:
                    V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4];
                    PC += 2;
                    break;

                // 8XY4: Adiciona Vy a Vx. VF é setado para 1 quando há um carry, e para 0 quando não há
                case 0x0004: {
                    uint16_t sum = V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0) >> 4];
                    V[0xF] = (sum > 255) ? 1 : 0;
                    V[(opcode & 0x0F00) >> 8] = sum & 0xFF; // Pega apenas os 8 bits inferiores
                    PC += 2;
                    break;
                }

                // 8XY5: Subtrai Vy de Vx. VF é setado para 0 quando há um borrow, e para 1 quando não há
                case 0x0005: {
                    uint8_t Vx = (opcode & 0x0F00) >> 8;
                    uint8_t Vy = (opcode & 0x00F0) >> 4;

                    // Set VF to 1 if Vx > Vy (no borrow), otherwise 0
                    V[0xF] = (V[Vx] >= V[Vy]) ? 1 : 0;

                    // Perform the subtraction
                    V[Vx] -= V[Vy];

                    PC += 2;
                    break;
                }
                // 0x8XY6: Desloca Vx para a direita. VF é setado para o bit que foi deslocado
                case 0x0006:
                    V[0xF] = V[(opcode & 0x0F00) >> 8] & 0x1;
                    V[(opcode & 0x0F00) >> 8] >>= 1;
                    PC += 2;
                    break;

                // 0x8XY7: Subtrai Vx de Vy. VF é setado para 0 quando há um borrow, e para 1 quando não há
                case 0x0007:
                    V[0xF] = (V[(opcode & 0x00F0) >> 4] >= V[(opcode & 0x0F00) >> 8]) ? 1 : 0;
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
                    PC += 2;
                    break;
                // 0x8XYE: Desloca Vx para a esquerda. VF é setado para o bit que foi deslocado
                case 0x000E:
                    V[0xF] = V[(opcode & 0x0F00) >> 8] >> 7;
                    V[(opcode & 0x0F00) >> 8] <<= 1;
                    PC += 2;
                    break;
                
                default:
                    cerr << "Opcode desconhecido: " << hex << opcode << endl;
                    PC += 2;
                    break;
            }
            break;
        // 9XY0: Pula a próxima instrução se Vx for igual a Vy
        case 0x9000:
            if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4]) {
                PC += 4;
            } else {
                PC += 2;
            }
            break;

        // ANNN: Armazena o endereço NNN no registrador I
        case 0xA000:
            I = opcode & 0x0FFF;
            PC += 2;
            break;
        // BNNN: Pula para o endereço NNN mais V0
        case 0xB000:
            PC = (opcode & 0x0FFF) + V[0];
            break;

        case 0xC000: {
            uint8_t Vx_index = (opcode & 0x0F00) >> 8;
            uint8_t nn = opcode & 0x00FF;
            V[Vx_index] = (rand() % 256) & nn;
            PC += 2;
            break;
        }
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
        case 0xE000:
            switch (opcode & 0x00FF) {
                // EX9E: Pula a próxima instrução se a tecla armazenada em Vx estiver pressionada
                case 0x009E:
                    if (keypad[V[(opcode & 0x0F00) >> 8]] != 0) {
                        PC += 4;
                    } else {
                        PC += 2;
                    }
                    break;

                // EXA1: Pula a próxima instrução se a tecla armazenada em Vx não estiver pressionada
                case 0x00A1:
                    if (keypad[V[(opcode & 0x0F00) >> 8]] == 0) {
                        PC += 4;
                    } else {
                        PC += 2;
                    }
                    break;

                default:
                    cerr << "Opcode desconhecido: " << hex << opcode << endl;
                    PC += 2;
                    break;
            }
            break;

        case 0xF000:
            switch (opcode & 0x00FF) {
                //FX07: Armazena o valor do timer de atraso em Vx
                case 0x0007:
                    V[(opcode & 0x0F00) >> 8] = DT;
                    PC += 2;
                    break;

                // FX0A: Espera por uma tecla pressionada e armazena o valor da tecla em Vx
                case 0x000A:
                    {
                        bool keyPressed = false;
                        for (int i = 0; i < 16; i++) {
                            if (keypad[i] != 0) {
                                V[(opcode & 0x0F00) >> 8] = i;
                                keyPressed = true;
                                break;
                            }
                        }
                        if (!keyPressed) {
                            return;
                        }
                        PC += 2;
                    }
                    break;

                // FX15: Define o timer de atraso para Vx
                case 0x0015:
                    DT = V[(opcode & 0x0F00) >> 8];
                    PC += 2;
                    break;

                // FX18: Define o timer de som para Vx
                case 0x0018:
                    ST = V[(opcode & 0x0F00) >> 8];
                    PC += 2;
                    break;

                // FX1E: Adiciona VX a I
                case 0x001E:
                    if (I + V[(opcode & 0x0F00) >> 8] > 0xFFF) {
                        V[0xF] = 1;
                    } else {
                        V[0xF] = 0;
                    }
                    I += V[(opcode & 0x0F00) >> 8];
                    PC += 2;
                    break;
                // FX29: Define I como o endereço da fonte de sprite para o dígito VX
                case 0x0029:
                    I = V[(opcode & 0x0F00) >> 8] * 5;
                    PC += 2;
                    break;
                // FX33: Armazena o valor binário codificado em BCD de Vx nos endereços I, I+1, e I+2
                case 0x0033:
                    memory[I]     = V[(opcode & 0x0F00) >> 8] / 100;
                    memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
                    memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 10);
                    PC += 2;
                    break;
                // FX55: Armazena os registros de V0 a Vx na memória a partir do endereço I
                case 0x0055:
                    for (int i = 0; i <= (opcode & 0x0F00) >> 8; i++) {
                        memory[I + i] = V[i];
                    }
                    //I += ((opcode & 0x0F00) >> 8) + 1;
                    PC += 2;
                    break;
                // FX65: Carrega os registros de V0 a Vx a partir da memória a partir do endereço I
                case 0x0065:
                    for (int i = 0; i <= (opcode & 0x0F00) >> 8; i++) {
                        V[i] = memory[I + i];
                    }
                    //I += ((opcode & 0x0F00) >> 8) + 1;
                    PC += 2;
                    break;
                default:
                    cerr << "Opcode desconhecido: " << hex << opcode << endl;
                    PC += 2;
                    break;
            }
            break;

        default:
            cerr << "Opcode desconhecido: " << hex << opcode << endl;
            PC += 2;
            break;
    }
}

void Chip8::updateTimers() {
    if (DT > 0) {
        DT--;
    }
    if (ST > 0) {
        ST--;
    }
}

void Chip8::loadROM(const string& romName) {
    Loader loader(romName);
    if (!loader.loadROM(memory)) {
        cerr << "Erro ao carregar ROM: " << romName << endl;
    }
}