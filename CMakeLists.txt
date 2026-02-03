cmake_minimum_required(VERSION 4.2)
project(420J12)

set(CMAKE_CXX_STANDARD 17)

add_executable(420J12
			   src/main.cpp
			   src/entity.cpp
			   src/characters.cpp
			   src/components.cpp
			   src/state.cpp
			   src/player.cpp)

target_link_libraries(420J12 PRIVATE SDL3 SDL3_image SDL3_ttf)

set(ASSETS_SRC_DIR ${CMAKE_SOURCE_DIR}/assets)
set(ASSETS_BINARY_DIR ${CMAKE_BINARY_DIR}/assets)
add_custom_target(
		assets ALL
		COMMAND ${CMAKE_COMMAND} -E remove_directory ${ASSETS_BINARY_DIR}
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${ASSETS_SRC_DIR} ${ASSETS_BINARY_DIR}
		COMMENT "Clearing and copying game assets into the binary directory."
		)
add_dependencies(${PROJECT_NAME} assets)
