#include <iostream>
#include <vector>
#include "cpu.hpp"
#include "ppu.hpp"

int main(int argc, char* argv[]) {
    CPU * cpu = new CPU;

    std::vector<std::uint8_t> rom = cpu->readROM("../res/Pong.ch8");
    //std::cout << rom.size() << "\n";


    for(int i = 0; i < rom.size()/2; i++){
        //std::cout << std::hex << cpu->fetchCode(rom, i) << '\n';
    }

    cpu->run();

    return 0;
}
