cmake_minimum_required(VERSION 2.6)
project(cagesim)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -fopenmp")

set(SOURCE_FILES src/main.cpp src/Game.h src/Player.h src/Strategy.h src/CongestionStrategy.h src/CongestionStrategy.cpp src/Player.cpp)
add_executable(cagesim ${SOURCE_FILES})