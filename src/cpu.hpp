#include <functional>
#include <map>
#include "ppu.hpp"

class CPU{
    private:
    std::map<std::string, std::function<void(uint16_t)>> opcodes;
    uint16_t opcode;
    uint16_t PC;                        //Program counter
    uint16_t I;                         //Index register
    uint8_t V[16]{};              //Registers
    uint8_t memory[4096]{};       //Ram
    uint8_t delay_timer;
    uint8_t sound_timer;
    std::vector<uint16_t> stack;        //Call stack
    uint16_t sp;                        //Stack pointer
    PPU * ppu;

    public:
    CPU();
    ~CPU();
    void run();
    void reset();
    std::vector<uint8_t> readROM(const std::string& path);
    void loadROM(const std::vector<uint8_t>& rom);
    uint16_t fetchCode(int index);
    void executeCode(uint16_t code);
    void loadFontSet();
    void writeRegister();
    void readRegister();
    void jumpAt(uint16_t dest);
    void callSub(uint16_t dest);
    void returnFromSub();
    void reg_dump(uint16_t bound);
    void reg_load(uint16_t bound);
    void removeSpaces(std::string& s);
    uint16_t xNNN(uint16_t code);
    uint16_t xxNN(uint16_t code);
    uint16_t xNxx(uint16_t code);
    uint16_t xxNx(uint16_t code);
    uint16_t xxxN(uint16_t code);
};