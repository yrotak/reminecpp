cmake_minimum_required(VERSION 3.5)

project(reminecpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

find_package(SDL2 REQUIRED)
find_package(SDL2_net REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(ZLIB)

add_subdirectory(${CMAKE_SOURCE_DIR}/src)