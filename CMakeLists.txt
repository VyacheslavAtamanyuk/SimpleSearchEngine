cmake_minimum_required(VERSION 3.28)
project(labwork11)

set(CMAKE_CXX_STANDARD 20)

add_executable(labwork11 main.cpp)

add_subdirectory(lib)

target_link_libraries(labwork11 Browser)

add_subdirectory(tests)

enable_testing()