cmake_minimum_required(VERSION 3.8)
project(ctypes)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES digit_sum_module.c)
add_executable(ctypes ${SOURCE_FILES})