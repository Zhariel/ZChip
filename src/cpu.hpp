#include <functional>
#include <map>
#include "storage.hpp"

class CPU{
    private:
    Storage storage;
    std::map<std::string, std::function<void(uint16_t)>> opcodes;

    public:
    CPU();
    ~CPU();
    void run();
    void restart();
    std::vector<uint8_t> loadRom(std::string path);
    uint16_t fetchCode(std::vector<uint8_t>& rom, int index);
    void executeCode(std::uint16_t code);
};