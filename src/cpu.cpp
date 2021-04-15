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
    stack = std::vector<uint16_t>();
    opcodes = {
        {"0.[^E].",[this](uint16_t code){;}},
        {"00E0",[this](uint16_t code){;}},
        {"00EE",[this](uint16_t code){;}},
        {"1...",[this](uint16_t code){jumpAt(xNNN(code));}},
        {"2...",[this](uint16_t code){;}},
        {"3...",[this](uint16_t code){;}},
        {"4...",[this](uint16_t code){;}},
        {"5..0",[this](uint16_t code){;}},
        {"6...",[this](uint16_t code){V[xNxx(code)] = xxNN(code);}},
        {"7...",[this](uint16_t code){V[xNxx(code)] += xxNN(code);}},
        {"8..0",[this](uint16_t code){V[xNxx(code)] = V[xxNx(code)];}},
        {"8..1",[this](uint16_t code){V[xNxx(code)] = V[xNxx(code)] | V[xxNx(code)];}},
        {"8..2",[this](uint16_t code){V[xNxx(code)] = V[xNxx(code)] & V[xxNx(code)];}},
        {"8..3",[this](uint16_t code){V[xNxx(code)] = V[xNxx(code)] ^ V[xxNx(code)];}},
        {"8..4",[this](uint16_t code){V[xNxx(code)] += V[xxNx(code)];}},
        {"8..5",[this](uint16_t code){V[xNxx(code)] -= V[xxNx(code)];}},
        {"8..6",[this](uint16_t code){V[15] = V[xNxx(code)] & 1; V[xNxx(code)] >>= 1;}},
        {"8..7",[this](uint16_t code){V[xNxx(code)] = V[xxNx(code)] - V[xNxx(code)];}}, //V15 if Borrow
        {"8..E",[this](uint16_t code){V[15] = V[xNxx(code)] & 0xEF; V[xNxx(code)] <<= 1;}},
        {"9..0",[this](uint16_t code){;}},
        {"A...",[this](uint16_t code){I = xNNN(code);}},
        {"B...",[this](uint16_t code){jumpAt(V[0] + xNNN(code));}},
        {"C...",[this](uint16_t code){V[xNxx(code)] = (rand() % 255) & xxNN(code);}},
        {"D...",[this](uint16_t code){;}}, //Draw sprite
        {"E.9E",[this](uint16_t code){;}},
        {"E.A1",[this](uint16_t code){;}},
        {"F.07",[this](uint16_t code){;}},
        {"F.0A",[this](uint16_t code){;}},
        {"F.15",[this](uint16_t code){;}}, //Timer VX
        {"F.18",[this](uint16_t code){;}}, //Timer VY
        {"F.1E",[this](uint16_t code){;}},
        {"F.29",[this](uint16_t code){;}},
        {"F.33",[this](uint16_t code){;}},
        {"F.35",[this](uint16_t code){;}},
        {"F.85",[this](uint16_t code){;}}
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
    std::stringstream stream;
    stream << std::hex << std::uppercase << code;

    std::string code_string = stream.str();

    for(auto const& kv : opcodes){
        if(std::regex_match(code_string, std::regex(kv.first))){
            kv.second(code);
            break;
        }
    }
}

auto CPU::xNNN(uint16_t code) -> uint16_t {
    return (code & 0x0FFF);
}

auto CPU::xxNN(uint16_t code) -> uint16_t {
    return (code & 0x00FF);
}

auto CPU::xNNx(uint16_t code) -> uint16_t {
    return (code & 0x0FF0) >> 4;
}

auto CPU::xNxx(uint16_t code) -> uint16_t {
    return (code & 0x0F00) >> 8;
}

auto CPU::xxNx(uint16_t code) -> uint16_t {
    return (code & 0x00F0) >> 4;
}

void CPU::jumpAt(uint16_t dest){
    stack.push_back(PC);
    PC = dest;
}

void CPU::returnFrom(){
    PC = stack[stack.size()-1];
    stack.pop_back();
}

void CPU::writeRegister() {

}

void CPU::readRegister() {

}
