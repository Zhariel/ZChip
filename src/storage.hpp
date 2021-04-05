#include <stdint.h>

class Storage{
    public:
    unsigned char ram[4096];
    unsigned char V[16];
    unsigned short I;   //index register
    unsigned short pc;  //program counter
};

enum class Memory_Map : uint16_t{
    interpreter_min = 0x000,
    interpreter_max = 0x1FF,
    font_set_min = 0x050,
    font_set_max = 0x0A0,
    working_min = 0x200,
    working_max = 0xFFF

};