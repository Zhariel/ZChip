#include <functional>
#include <map>

class CPU{
    private:
    std::map<std::string, std::function<void(uint16_t)>> opcodes;
    uint16_t opcode;
    uint16_t PC;                        //Program counter
    uint16_t I;                         //Index register
    bool key[16]{};                     //Keymap
    unsigned char V[16]{};              //Registers
    unsigned char memory[4096]{};       //Ram
    unsigned char delay_timer;
    unsigned char sound_timer;
    std::vector<uint16_t> stack;        //Call stack
    uint16_t sp;                        //Stack pointer

    public:
    CPU();
    ~CPU();
    void run();
    void reset();
    std::vector<uint8_t> readROM(const std::string& path);
    void loadROM(const std::vector<uint8_t>& rom);
    uint16_t fetchCode(std::vector<uint8_t>& rom, int index);
    void executeCode(uint16_t code);
    void writeRegister();
    void readRegister();
    void jumpAt(uint16_t dest);
    void callSub(uint16_t dest);
    void returnFromSub();
    void reg_dump(uint16_t bound);
    void reg_load(uint16_t bound);
    uint16_t xNNN(uint16_t code);
    uint16_t xxNN(uint16_t code);
    uint16_t xNxx(uint16_t code);
    uint16_t xxNx(uint16_t code);
    void removeSpaces(std::string& s);
};