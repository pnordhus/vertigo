cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

project(vertigo)

find_package(Qt4 4.6.0 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(OpenAL REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 11)
if(${CMAKE_VERSION} VERSION_LESS 3.1)
    add_compile_options($<$<CXX_COMPILER_ID:Clang>:-std=c++11>)
    add_compile_options($<$<CXX_COMPILER_ID:GNU>:-std=c++11>)
endif()

add_subdirectory(data)
add_subdirectory(src)
