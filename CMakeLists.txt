cmake_minimum_required(VERSION 3.29)
project(BiomeMaker)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(BiomeMaker ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE src ${CMAKE_SOURCE_DIR}/external/raylib)
target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/external/raylib)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib winmm gdi32 opengl32)