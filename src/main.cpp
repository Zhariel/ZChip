#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include "cpu.hpp"

int main(int argc, char* argv[]) {
    CPU * cpu = new CPU;

    std::vector<std::uint8_t> rom = cpu->readROM("../res/Pong.ch8");
    //std::cout << rom.size() << "\n";


    for(int i = 0; i < rom.size()/2; i++){
        //std::cout << std::hex << cpu->fetchCode(rom, i) << '\n';
    }

    uint8_t test = 0 - 1;
    std::cout << int(test);

    return 0;
}
