#pragma once

#include <string>
#include <array>

using namespace std;

class Loader {
public:
    Loader(const string& romName);

    bool loadROM(array<uint8_t, 4096>& memory);
    
private:
    string romName;
};

