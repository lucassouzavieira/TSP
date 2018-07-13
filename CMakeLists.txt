cmake_minimum_required(VERSION 3.10)
project(CaixeroViajante)

set(CMAKE_CXX_STANDARD 14)

file(GLOB INCLUDE_FILE
        include/*.c
        include/*.h
        include/*.cpp
        include/*.hpp)

file(GLOB SRC_FILES
        src/*.c
        src/*.cpp)

include_directories(
        include
        src
)

add_executable(CaixeroViajante main.cpp ${INCLUDE_FILES} ${SRC_FILES})