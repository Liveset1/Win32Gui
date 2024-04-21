cmake_minimum_required(VERSION 3.29)

project(Win32Gui C)
set(CMAKE_C_STANDARD 17)

file(GLOB_RECURSE C_SOURCES "src/*.c")
file(GLOB_RECURSE C_HEADERS "src/*.h")

option(WIN32GUI_BUILD_DLL "Build shared library instead of static" OFF)

if(WIN32GUI_BUILD_DLL)
    add_library(${PROJECT_NAME} SHARED include/Win32Gui.h ${C_SOURCES} ${C_HEADERS})
else()
    add_library(${PROJECT_NAME} STATIC include/Win32Gui.h ${C_SOURCES} ${C_HEADERS})
endif()

# Set WIN32GUI_BUILD_DLL to control how Win32Gui is linked
if(WIN32GUI_BUILD_DLL)
    target_compile_definitions(Win32Gui PUBLIC WIN32GUI_DLL)
else()
    target_compile_definitions(Win32Gui PUBLIC WIN32GUI_STATIC)
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC user32.lib gdi32.lib kernel32.lib)