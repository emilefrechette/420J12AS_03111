cmake_minimum_required(VERSION 3.10.0)
project(Steamworks VERSION 0.1.0 LANGUAGES C CXX)

add_executable(${PROJECT_NAME}
			   src/main.cpp
			   src/app.cpp
               src/entity.cpp
			   )

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3 SDL3_ttf steam_api64)
target_include_directories(${PROJECT_NAME} PRIVATE src)
