cmake_minimum_required(VERSION 3.29)
project(BiomeMaker)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(BiomeMaker ${SOURCES})

find_package(raylib CONFIG REQUIRED)

target_include_directories(${PROJECT_NAME} PRIVATE src)

target_link_libraries(${PROJECT_NAME} raylib)