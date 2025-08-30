#include "loader.h"
#include <fstream>
#include <iostream>
Loader::Loader(const string& romName) : romName(romName) {}

bool Loader::loadROM(std::array<uint8_t, 4096>& memory) {
    std::ifstream romFile(romName, std::ios::binary | std::ios::ate); // Abre como binário e vai para o final
    if (!romFile) {
        std::cerr << "Erro ao abrir ROM: " << romName << std::endl;
        return false;
    }

    streamsize size = romFile.tellg(); // Captura o tamanho do arquivo
    romFile.seekg(0, ios::beg);
    // Retorna ao inicio do arquivo para leitura

    if (size > 4096 - 0x200) {
        std::cerr << "ROM muito grande: " << romName << std::endl;
        return false;
    }

    romFile.read(reinterpret_cast<char*>(&memory[0x200]), size);
    if (!romFile) {
        std::cerr << "Erro ao ler ROM: " << romName << std::endl;
        return false;
    }

    return true;
}
