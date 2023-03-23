cmake_minimum_required(VERSION 3.13)  # CMake version check
project(ComputeShader)          # Create project "AppToGIF_Demo"
set(CMAKE_CXX_STANDARD 17)            # Enable c++17 standard

# Add main file
set(SOURCE_FILES 
    src/main.cpp)
add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})

# ---- Link libraries ----
# AppToGIF
include_directories(
    ${CMAKE_SOURCE_DIR}/vendor/inc)
link_directories(
    ${CMAKE_SOURCE_DIR}/vendor/inc)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE 
        ${CMAKE_SOURCE_DIR}/vendor/build/libglad.a
        ${CMAKE_SOURCE_DIR}/vendor/lib/libglfw.3.3.dylib
        )