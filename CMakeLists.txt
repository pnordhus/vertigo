cmake_minimum_required(VERSION 2.6)

project(vertigo)

find_package(Qt4 4.6.0 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(OpenAL REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
include_directories(${CMAKE_SOURCE_DIR}/src)

add_subdirectory(src)
