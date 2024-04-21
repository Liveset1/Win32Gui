cmake_minimum_required(VERSION 3.29)

project(Win32Gui C)
set(CMAKE_C_STANDARD 17)

file(GLOB_RECURSE C_SOURCES "src/*.c")
file(GLOB_RECURSE C_HEADERS "src/*.h")

option(WIN32GUI_BUILD_DLL "Build shared library instead of static" OFF)

if(WIN32GUI_BUILD_DLL)
    add_library(${PROJECT_NAME} SHARED ${C_SOURCES} ${C_HEADERS})
else()
    add_library(${PROJECT_NAME} STATIC ${C_SOURCES} ${C_HEADERS})
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC user32 gdi32 kernel32)