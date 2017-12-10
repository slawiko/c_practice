cmake_minimum_required(VERSION 3.8)
project(digest)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c99")


include_directories(/usr/local/Cellar/zlib/1.2.11/include)
link_libraries(z)
set(SOURCE_FILES main.c calculate.c calculate.h print.c print.h structure.h)
add_executable(digest ${SOURCE_FILES} calculate.h)
