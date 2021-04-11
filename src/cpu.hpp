#include <functional>
#include <map>

class CPU{
    private:
    std::map<std::string, std::function<void(uint16_t)>> opcodes;
    unsigned short opcode;
    unsigned short PC;                  //Program counter
    unsigned short I;                   //Index register
    unsigned char V[16]{};              //Registers
    unsigned char key[16]{};            //Keymap
    unsigned short stack[16]{};         //Call stack
    unsigned char ram[4096]{};          //Ram
    unsigned short sp;                  //Stack pointer

    public:
    CPU();
    ~CPU();
    void run();
    void reset();
    std::vector<uint8_t> loadRom(const std::string& path);
    uint16_t fetchCode(std::vector<uint8_t>& rom, int index);
    void executeCode(std::uint16_t code);
    void writeRegister(int v);
    void readRegister();
    void jumpAt();
    void storeStack();
    void popStack();
};