cmake_minimum_required(VERSION 2.6)
project(cagesim)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -fopenmp")

add_subdirectory("puddi")

include_directories(${cagesim_SOURCE_DIR}/puddi/include ${cagesim_SOURCE_DIR}/include)

set(SOURCE_FILES src/main.cpp src/Game.h src/Player.h src/Strategy.h src/CongestionStrategy.h src/CongestionStrategy.cpp src/Player.cpp src/Plot.h src/Game.cpp)
add_executable(cagesim ${SOURCE_FILES})

target_link_libraries(cagesim puddi gomp GLEW GL SDL2 png jpeg z SDL2_image assimp)