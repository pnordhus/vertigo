cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

project(vertigo)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# disable SDL_main wrapper function
set(SDL2_BUILDING_LIBRARY 1)
add_definitions(-DSDL_MAIN_HANDLED)

find_package(SDL2 REQUIRED)
find_package(Qt5OpenGL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(OpenAL REQUIRED)
find_package(glm REQUIRED)
add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_STANDARD 11)
if(${CMAKE_VERSION} VERSION_LESS 3.1)
    add_compile_options($<$<CXX_COMPILER_ID:Clang>:-std=c++11>)
    add_compile_options($<$<CXX_COMPILER_ID:GNU>:-std=c++11>)
endif()

add_subdirectory(data)
add_subdirectory(src)
