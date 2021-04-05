cmake_minimum_required(VERSION 3.0.0)
project(chip-8 VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(chip-8 src/main.cpp)

target_link_libraries(chip-8 SDL2main SDL2)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
