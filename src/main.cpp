#include <iostream>
//#include <SDL2/SDL.h>
#include <stdio.h>
#include <vector>
#include "cartridge.cpp"
#include "storage.cpp"

int main(int argc, char* argv[]) {

    std::vector<std::uint8_t> rom = loadRom("../res/Pong.ch8");
 

    for(int i = 0; i < rom.size()/2; i++){
        std::cout << std::hex << fetch(rom, i) << " " << std::dec << i << '\n';
    }

    
    return 0;
}