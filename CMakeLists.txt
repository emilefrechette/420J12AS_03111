cmake_minimum_required(VERSION 3.10.0)
project(Database VERSION 0.1.0 LANGUAGES C CXX)

add_executable(${PROJECT_NAME}
			   src/main.cpp
			   src/app.cpp
			   src/db.cpp
               src/entity.cpp
               src/components/box.cpp
               src/components/color.cpp
			   src/components/render.cpp
			   src/components/text.cpp
			   src/components/transform.cpp
			   )

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3 SDL3_ttf sqlite3)
target_include_directories(${PROJECT_NAME} PRIVATE src)
