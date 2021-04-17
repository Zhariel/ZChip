#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include "cpu.hpp"

int main(int argc, char* argv[]) {
    CPU * cpu = new CPU;

    std::vector<std::uint8_t> rom = cpu->loadRom("../res/Pong.ch8");
    std::cout << rom.size() << "\n";


    for(int i = 0; i < rom.size()/2; i++){
        //std::cout << std::hex << cpu->fetchCode(rom, i) << " " << std::to_string(0x6b0c) << '\n';
    }

    cpu->executeCode(0x0000);

    return 0;
}
