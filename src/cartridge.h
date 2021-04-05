#include <fstream>

std::vector<uint8_t> loadRom(std::string path){
    std::ifstream rom(path, std::ios::binary);
    if(!rom){
        std::cout << "Game not found.\n";
    }

    return {std::istreambuf_iterator<char>(rom), {}};
}

uint16_t fetch(std::vector<uint8_t>& rom, int index){
    index *= 2;
    return rom[index] << 8 | rom[index + 1];
}