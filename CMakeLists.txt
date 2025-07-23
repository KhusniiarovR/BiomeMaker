cmake_minimum_required(VERSION 3.29)
project(BiomeMaker)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(BiomeMaker ${SOURCES})

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O3 -march=native -mtune=native)
endif()

# === [ Установка Raylib по платформе ] ===
if(WIN32)
    set(RAYLIB_DIR "${CMAKE_SOURCE_DIR}/external/raylib/windows")
    set(RAYLIB_LIB "${RAYLIB_DIR}/lib/libraylib.a")
    set(RAYLIB_EXTRA_LIBS winmm gdi32 opengl32)
elseif(UNIX)
    set(RAYLIB_DIR "${CMAKE_SOURCE_DIR}/external/raylib/linux")
    set(RAYLIB_LIB "${RAYLIB_DIR}/lib/libraylib.a")
    set(RAYLIB_EXTRA_LIBS m pthread dl GL X11)
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

# === [ Подключение Raylib как внешней библиотеки ] ===
add_library(raylib STATIC IMPORTED)
set_target_properties(raylib PROPERTIES
    IMPORTED_LOCATION "${RAYLIB_LIB}"
    INTERFACE_INCLUDE_DIRECTORIES "${RAYLIB_DIR}/include"
)

# === [ Настройка include и линковки ] ===
target_include_directories(BiomeMaker PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${RAYLIB_DIR}/include
)

target_link_libraries(BiomeMaker PRIVATE raylib ${RAYLIB_EXTRA_LIBS})
