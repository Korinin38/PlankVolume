cmake_minimum_required(VERSION 3.16)
project(PlankVolume)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static -lpthread")
set(SOURCE_FILES main.cpp)

add_executable(PlankVolume ${SOURCE_FILES})

target_link_libraries(PlankVolume ${CMAKE_EXE_LINKER_FLAGS})