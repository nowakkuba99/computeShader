# CMake Version
cmake_minimum_required(VERSION 3.13)
# Project Name
project(glad)
# C++17 Standard
set(CMAKE_CXX_STANDARD 17)
# Add library
add_library(${CMAKE_PROJECT_NAME} inc/glad/glad.h src/glad.c)
target_include_directories(glad PUBLIC inc/)