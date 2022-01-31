#include <iostream>
#include <vector>
#include "cpu.hpp"

int main(int argc, char* argv[]) {
    auto cpu = std::make_unique<CPU>("../res/test_opcode.ch8");
    cpu->run();

    return 0;
}
