#include <iostream>
#include <functional>
#include <fstream>
#include <map>
#include <regex>
#include <cstdint>
#include <iomanip>
//#include <boost/algorithm/string/replace.hpp>
#include "cpu.hpp"

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
    ppu = new PPU;
    loadROM(readROM("../res/Pong.ch8"));
    loadFontSet();
    opcodes = {
        {"0.[^E].",[this](uint16_t code){;}},
        {"00E0",[this](uint16_t code){ppu->cls(0, 0, 0, 0xFF);}},
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
        {"D...",[this](uint16_t code){ppu->draw_sprite(memory, V[xNxx(code)], V[xxNx(code)], I, xxxN(code)+1);}}, //Draw sprite
        {"E.9E",[this](uint16_t code){if(ppu->key[xNxx(code)]) PC += 2;}},
        {"E.A1",[this](uint16_t code){if(!ppu->key[xNxx(code)]) PC += 2;}},
        {"F.07",[this](uint16_t code){V[xNxx(code)] = delay_timer;}},
        {"F.0A",[this](uint16_t code){V[xNxx(code)] = ppu->input();}},
        {"F.15",[this](uint16_t code){delay_timer = V[xNxx(code)];}},
        {"F.18",[this](uint16_t code){sound_timer = V[xNxx(code)];}},
        {"F.1E",[this](uint16_t code){I += V[xNxx(code)];}},
        {"F.29",[this](uint16_t code){I = ppu->fontset[V[xNxx(code)]];}},
        {"F.33",[this](uint16_t code){;}},
        {"F.35",[this](uint16_t code){reg_dump(V[xNxx(code)]);}},
        {"F.65",[this](uint16_t code){reg_load(V[xNxx(code)]);}}
    };
}

CPU::~CPU(){
    delete[] &PC, &I, &V, &stack, &sp, &memory, &opcodes;
}

void CPU::run() {
    ppu->cls(0, 0, 0, 0xFF);
    ppu->alive = true;
//    for(int i = 0x050; i < 0x0A0; i++){
//        uint16_t e = memory[i];
//        std::cout << std::hex << e << " ";
//    }
    for(int i = 0; i < 15; i++){
        ppu->draw_sprite(memory, i*5, 0, FONT_SET_MIN + i*5, 5);
    }
    for(;;){

        ppu->processEvent();
        if(delay_timer > 0) {
            delay_timer--;}
        if(sound_timer > 0) {
            if (sound_timer == 1)
                std::cout << "Le beep\n";
                sound_timer--;}
    }
    ppu->destroy();
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
        memory[WORKING_MIN + i] = rom[i];
    }
}

void CPU::loadFontSet(){
//    std::cout << "loading" << std::endl;
    for(int i = FONT_SET_MIN, k = 0; i < ppu->fontset.size(); i++, k++){
//        std::cout << ppu->fontset[k] << " ";
        memory[i] = ppu->fontset[k];
    }
}

auto CPU::fetchCode(int index) -> uint16_t{
    return memory[index] << 8 | memory[index + 1];
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

auto CPU::xxxN(uint16_t code) -> uint16_t {
    return (code & 0x000F);
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