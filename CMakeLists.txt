cmake_minimum_required(VERSION 3.29)
project(BiomeMaker)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(BiomeMaker ${SOURCES})

add_subdirectory(external/raylib)

target_include_directories(BiomeMaker PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/external/raylib/src
)

target_link_libraries(BiomeMaker PRIVATE raylib)

if(WIN32)
    target_link_libraries(BiomeMaker PRIVATE winmm gdi32 opengl32)
elseif(UNIX)
    find_package(X11 REQUIRED)
    target_link_libraries(BiomeMaker PRIVATE
        m pthread dl X11::X11 GL
    )
endif()
