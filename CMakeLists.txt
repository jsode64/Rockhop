cmake_minimum_required(VERSION 3.0)
project(rockhop)

set(CMAKE_CXX_STANDARD 23)

add_compile_options(-O3 -Wall -Wextra -Wpedantic)

# -fno-omit-frame-pointer

file(GLOB SOURCES "./src/*.cpp")
add_executable(rockhop ${SOURCES})
target_include_directories(rockhop PRIVATE "./src")
