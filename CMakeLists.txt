cmake_minimum_required(VERSION 3.10.0)
project(VSCode VERSION 0.1.0 LANGUAGES C CXX)

add_executable(VSCode simple_example.cpp)

target_link_libraries(VSCode PRIVATE SDL3)