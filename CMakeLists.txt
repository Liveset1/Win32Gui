cmake_minimum_required(VERSION 3.29)

project(Win32Gui C)
set(CMAKE_C_STANDARD 17)

file(GLOB_RECURSE C_SOURCES "src/*.c")
file(GLOB_RECURSE C_HEADERS "src/*.h")

add_library(${PROJECT_NAME} STATIC ${C_SOURCES} ${C_HEADERS})

if (MINGW)
    target_link_libraries(${PROJECT_NAME} PUBLIC user32.lib gdi32.lib kernel32.lib)
    # target_link_libraries(${PROJECT_NAME} PUBLIC libuser32.a libgdi32.a libkernel32.a)
elseif(MSVC)
    target_link_libraries(${PROJECT_NAME} PUBLIC user32.lib gdi32.lib kernel32.lib)
endif()