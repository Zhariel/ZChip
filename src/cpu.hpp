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
};