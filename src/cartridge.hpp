#include <iostream>
#include <vector>
#include <stdint.h>


class Cartridge{
    private:
    std::string file;

    public:
    Cartridge();
    ~Cartridge();
    std::vector<uint8_t> loadRom(std::string path);
    uint16_t fetchCode(std::vector<uint8_t>& rom, int index);
    void inputName();
};