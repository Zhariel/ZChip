#include <iostream>
#include <functional>
#include <fstream>
#include <map>
#include <regex>
#include "cpu.hpp"

CPU::CPU(){
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
    delete[] &opcodes;
}

auto CPU::loadRom(std::string path) -> std::vector<uint8_t>{
    std::ifstream rom(path, std::ios::binary);
    if(!rom){
        std::cout << "Game not found.\n";
        return {};
    }

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