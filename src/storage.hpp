#include <stdint.h>

#define INTERPRETER_MIN 0x000
#define INTERPRETER_MAX 0x1FF
#define FONT_SET_MIN 0x050
#define FONT_SET_MAX 0x0A0
#define WORKING_MIN 0x200
#define WORKING_MAX 0xFFF

class Storage{
    public:
    unsigned char ram[4096];
    unsigned char V[16];
    unsigned short I;   //index register
    unsigned short pc;  //program counter

    Storage();
    ~Storage();
    void writeRamAt();
    uint16_t readRamAt();
    void writeRegister(int register);
    void readRegister();
};

// enum class Memory_Map : uint16_t{
//     interpreter_min = 0x000,
//     interpreter_max = 0x1FF,
//     font_set_min = 0x050,
//     font_set_max = 0x0A0,
//     working_min = 0x200,
//     working_max = 0xFFF

// };