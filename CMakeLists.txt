cmake_minimum_required(VERSION 3.5)
project(TankWar)

# files to be compiled, separated by spaces
set(SOURCE_FILES src/main.cpp src/terminal.cpp src/Entity.cpp src/Field.cpp src/AI.cpp)

# Generate compile_commands.json
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# name of the executable
add_executable(tankwar ${SOURCE_FILES})

# Set the C++ Standard (don't modify)
set(CMAKE_CXX_STANDARD 17)

# target_link_libraries(p3 glu32 freeglut opengl32)

include_directories(.)

# Set the compiling flags (don't modify)
set(CMAKE_CXX_FLAGS "-O2")
# set(CMAKE_CXX_FLAGS "-Werror -Wpedantic -Wall -Wextra -Wvla -Wno-unused-result -Wconversion")
# -Werror -Wpedantic -Wall -Wextra -Wvla -Wno-unused-result -Wconversion

