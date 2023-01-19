cmake_minimum_required(VERSION 3.21)
project(pneumo_model C)

set(CMAKE_C_STANDARD 99)

add_definitions(-DPNEUMO_SIMULATE -DPNEUMO_DEBUG)
add_executable(pneumo_model main.c automate.c automate.h)
