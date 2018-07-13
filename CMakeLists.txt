cmake_minimum_required(VERSION 3.10)
project(CaixeroViajante)

set(CMAKE_CXX_STANDARD 14)

file(GLOB INCLUDE_FILES
        include/*.c
        include/*.h)

file(GLOB SRC_FILES
        src/*.c
        src/*.cpp)

include_directories(
        include
        src
)

add_executable(CaixeroViajante main.cpp ${SRC_FILES} ${INCLUDE_FILES})
set_property(TARGET CaixeroViajante PROPERTY C_STANDARD 99)