#include <iostream>
#include <functional>
#include <fstream>
#include <map>
#include <regex>
#include "cpu.hpp"
#include <cstdint>

#define INTERPRETER_MIN 0x000
#define INTERPRETER_MAX 0x1FF
#define FONT_SET_MIN 0x050
#define FONT_SET_MAX 0x0A0
#define WORKING_MIN 0x200
#define WORKING_MAX 0xFFF

CPU::CPU(){
    opcode = 0;
    I = 0x000;
    PC = 0x200;
    sp = 0;
    std::fill(stack, stack + 15, -1);
    opcodes = {
        {"0.[^E].",[](uint16_t code){}},
        {"00E0",[](uint16_t code){}},
        {"00EE",[](uint16_t code){}},
        {"1...",[](uint16_t code){}},
        {"2...",[](uint16_t code){}},
        {"3...",[](uint16_t code){}},
        {"4...",[](uint16_t code){}},
        {"5..0",[](uint16_t code){}},
        {"6...",[](uint16_t code){}},
        {"7...",[](uint16_t code){}},
        {"8..0",[](uint16_t code){}},
        {"8..1",[](uint16_t code){}},
        {"8..2",[](uint16_t code){}},
        {"8..3",[](uint16_t code){}},
        {"8..4",[](uint16_t code){}},
        {"8..5",[](uint16_t code){}},
        {"8..6",[](uint16_t code){}},
        {"8..7",[](uint16_t code){}},
        {"8..E",[](uint16_t code){}},
        {"9..0",[](uint16_t code){}},
        {"A...",[](uint16_t code){}},
        {"B...",[](uint16_t code){}},
        {"C...",[](uint16_t code){}},
        {"D...",[](uint16_t code){}},
        {"E.9E",[](uint16_t code){}},
        {"E.A1",[](uint16_t code){}},
        {"F.07",[](uint16_t code){}},
        {"F.0A",[](uint16_t code){}},
        {"F.15",[](uint16_t code){}},
        {"F.18",[](uint16_t code){}},
        {"F.1E",[](uint16_t code){}},
        {"F.29",[](uint16_t code){}},
        {"F.33",[](uint16_t code){}},
        {"F.35",[](uint16_t code){}},
        {"F.85",[](uint16_t code){}}
    };

}

CPU::~CPU(){
    delete[] &PC, &I, &V, &key, &stack, &sp, &ram, &opcodes;
}

auto CPU::loadRom(const std::string& path) -> std::vector<uint8_t>{
    std::ifstream rom(path, std::ios::binary);
    if(!rom){
        std::cout << "Game not found.\n";
        return {};
    }
    std::cout << "Rom loaded.\n";

    return {std::istreambuf_iterator<char>(rom), {}};
}

auto CPU::fetchCode(std::vector<uint8_t>& rom, int index) -> uint16_t{
    index *= 2;
    return rom[index] << 8 | rom[index + 1];
}

void CPU::executeCode(uint16_t code){
    for(auto const& kv : opcodes){
        if(std::regex_match(std::to_string(code), std::regex(kv.first))){
            kv.second(code);
            break;
        }
    }
}

void CPU::writeRegister(int v) {

}

void CPU::readRegister() {

}
