cmake_minimum_required(VERSION 3.28.1)
project(IHateSoup)

file(GLOB SOURCES ./src/*.cpp)
add_executable(main ${SOURCES})
