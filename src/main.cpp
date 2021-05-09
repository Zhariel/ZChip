#include <iostream>
#include <vector>
#include "cpu.hpp"

int main(int argc, char* argv[]) {
    CPU * cpu = new CPU("../res/test_opcode.ch8");

//    std::vector<std::uint8_t> rom = cpu->readROM("../res/Pong.ch8");

//    for(int i = 0x200; i < rom.size() + 0x200; i+=2) {
//        std::cout << "code : " << std::hex << cpu->fetchCode(i) << '\n';
//    }
    cpu->run();

    return 0;
}
