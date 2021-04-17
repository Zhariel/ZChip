#include <iostream>
#include <functional>
#include <boost/algorithm/string/replace.hpp>
#include <fstream>
#include <map>
#include <regex>
#include "cpu.hpp"
#include <cstdint>
#include <iomanip>

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
    delay_timer = sound_timer = 0;
    stack = std::vector<uint16_t>();
    opcodes = {
        {"0.[^E].",[this](uint16_t code){;}},
        {"00E0",[this](uint16_t code){;}},  //CLS
        {"00EE",[this](uint16_t code){returnFromSub();}},
        {"1...",[this](uint16_t code){jumpAt(xNNN(code));}},
        {"2...",[this](uint16_t code){callSub(xNNN(code));}},
        {"3...",[this](uint16_t code){if (V[xNxx(code)] == xxNN(code)) PC += 2;}},
        {"4...",[this](uint16_t code){if (V[xNxx(code)] != xxNN(code)) PC += 2;}},
        {"5..0",[this](uint16_t code){if (V[xNxx(code)] == V[xxNx(code)]) PC += 2;}},
        {"6...",[this](uint16_t code){V[xNxx(code)] = xxNN(code);}},
        {"7...",[this](uint16_t code){V[xNxx(code)] += xxNN(code);}},
        {"8..0",[this](uint16_t code){V[xNxx(code)] = V[xxNx(code)];}},
        {"8..1",[this](uint16_t code){V[xNxx(code)] = V[xNxx(code)] | V[xxNx(code)];}},
        {"8..2",[this](uint16_t code){V[xNxx(code)] = V[xNxx(code)] & V[xxNx(code)];}},
        {"8..3",[this](uint16_t code){V[xNxx(code)] = V[xNxx(code)] ^ V[xxNx(code)];}},
        {"8..4",[this](uint16_t code){uint8_t val = (V[xNxx(code)] += V[xxNx(code)]); V[xNxx(code)] = val; V[0xF] = V[xNxx(code)] + V[xxNx(code)] > 0xFF ? 01 : 00;}},
        {"8..5",[this](uint16_t code){uint8_t val = (V[xNxx(code)] - V[xxNx(code)]); V[xNxx(code)] = val; V[0xF] = V[xNxx(code)] < V[xxNx(code)] ? 01 : 00;}},
        {"8..6",[this](uint16_t code){V[0xF] = V[xNxx(code)] & 1; V[xNxx(code)] >>= 1;}},
        {"8..7",[this](uint16_t code){uint8_t val = (V[xxNx(code)] - V[xNxx(code)]); V[xNxx(code)] = val; V[0xF] = V[xxNx(code)] < V[xNxx(code)] ? 01 : 00;}},
        {"8..E",[this](uint16_t code){V[0xF] = V[xNxx(code)] & 0xEF; V[xNxx(code)] <<= 1;}},
        {"9..0",[this](uint16_t code){if (V[xNxx(code)] != V[xxNx(code)]) PC += 2;}},
        {"A...",[this](uint16_t code){I = xNNN(code);}},
        {"B...",[this](uint16_t code){jumpAt(V[0x0] + xNNN(code));}},
        {"C...",[this](uint16_t code){V[xNxx(code)] = (rand() % 0xFF) & xxNN(code);}},
        {"D...",[this](uint16_t code){;}}, //Draw sprite
        {"E.9E",[this](uint16_t code){;}},
        {"E.A1",[this](uint16_t code){;}},
        {"F.07",[this](uint16_t code){V[xNxx(code)] = delay_timer;}},
        {"F.0A",[this](uint16_t code){;}},
        {"F.15",[this](uint16_t code){delay_timer = V[xNxx(code)];}},
        {"F.18",[this](uint16_t code){sound_timer = V[xNxx(code)];}},
        {"F.1E",[this](uint16_t code){I += V[xNxx(code)];}},
        {"F.29",[this](uint16_t code){;}},
        {"F.33",[this](uint16_t code){;}},
        {"F.35",[this](uint16_t code){reg_dump(V[xNxx(code)]);}},
        {"F.65",[this](uint16_t code){reg_load(V[xNxx(code)]);}}
    };

}

CPU::~CPU(){
    delete[] &PC, &I, &V, &key, &stack, &sp, &memory, &opcodes;
}

auto CPU::readROM(const std::string& path) -> std::vector<uint8_t>{
    std::ifstream rom(path, std::ios::binary);
    if(!rom){
        std::cout << "Game not found.\n";
        return {};
    }
    std::cout << "Rom loaded.\n";

    return {std::istreambuf_iterator<char>(rom), {}};
}

void CPU::loadROM(const std::vector<uint8_t>& rom) {
    for(int i = 0; i < rom.size(); i++){
        memory[0x200 + i] = rom[i];
    }
}

auto CPU::fetchCode(std::vector<uint8_t>& rom, int index) -> uint16_t{
    index *= 2;
    return rom[index] << 8 | rom[index + 1];
}

void CPU::executeCode(uint16_t code){
    std::stringstream stream;
    stream << std::hex << std::uppercase << std::setw(4) << code;

    std::string code_string = stream.str();
    removeSpaces(code_string);

    for(auto const& kv : opcodes){
        if(std::regex_match(code_string, std::regex(kv.first))){
            kv.second(code);
            break;
        }
    }
}

void CPU::reg_dump(uint16_t bound){
    uint16_t j = 0;
    uint16_t address = I;

    while(j <= bound){
        memory[address] = V[j];
        j++;
        address++;
    }
}

void CPU::reg_load(uint16_t bound){
    uint16_t j = 0;
    uint16_t address = I;

    while(j <= bound){
         V[j] = memory[address];
        j++;
        address++;
    }
}

auto CPU::xNNN(uint16_t code) -> uint16_t {
    return (code & 0x0FFF);
}

auto CPU::xxNN(uint16_t code) -> uint16_t {
    return (code & 0x00FF);
}

auto CPU::xNxx(uint16_t code) -> uint16_t {
    return (code & 0x0F00) >> 8;
}

auto CPU::xxNx(uint16_t code) -> uint16_t {
    return (code & 0x00F0) >> 4;
}

void CPU::jumpAt(uint16_t dest){
    PC = dest;
}

void CPU::callSub(uint16_t dest){
    stack.push_back(PC);
    PC = dest;
}

void CPU::returnFromSub(){
    PC = stack[stack.size()-1];
    stack.pop_back();
}

void CPU::writeRegister() {

}

void CPU::readRegister() {

}

void CPU::removeSpaces(std::string& s){
    int i = 0;
    while(s[i] == ' ') {
        s[i] = '0';
        i++;
    }
}
