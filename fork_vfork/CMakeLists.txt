cmake_minimum_required(VERSION 3.10)
project(fork_vfork C)

set(CMAKE_C_STANDARD 99)

add_executable(fork_vfork fork.c vfork.c)