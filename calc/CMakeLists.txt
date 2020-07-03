cmake_minimum_required(VERSION 3.16)
project(calc C)

set(CMAKE_C_STANDARD 99)

add_executable(calc main.c)
target_link_libraries(calc PRIVATE m)