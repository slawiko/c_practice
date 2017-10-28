cmake_minimum_required(VERSION 3.8)
project(master_transpose)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c)
add_executable(master_transpose ${SOURCE_FILES})