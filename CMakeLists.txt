cmake_minimum_required(VERSION 3.29)
project(BiomeMaker)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(BiomeMaker ${SOURCES})

set(RAYLIB_DIR "${CMAKE_SOURCE_DIR}/external/raylib")
set(RAYLIB_INCLUDE_DIR "${RAYLIB_DIR}/include")
set(RAYLIB_LIB "${RAYLIB_DIR}/lib/libraylib.a")

add_library(raylib STATIC IMPORTED)
set_target_properties(raylib PROPERTIES
    IMPORTED_LOCATION "${RAYLIB_LIB}"
    INTERFACE_INCLUDE_DIRECTORIES "${RAYLIB_INCLUDE_DIR}"
)

target_link_libraries(BiomeMaker PRIVATE raylib)

target_include_directories(BiomeMaker PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

if(WIN32)
    target_link_libraries(BiomeMaker PRIVATE winmm gdi32 opengl32)
elseif(UNIX)
    find_package(X11 REQUIRED)
    target_link_libraries(BiomeMaker PRIVATE
        m pthread dl X11::X11 GL
    )
endif()

