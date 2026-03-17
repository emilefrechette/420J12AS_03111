cmake_minimum_required(VERSION 3.10.0)
project(AsteroidTest VERSION 0.1.0 LANGUAGES C CXX)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3 SDL3_image)
