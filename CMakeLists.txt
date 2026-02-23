cmake_minimum_required(VERSION 3.21)
project(420J12)

set(CMAKE_CXX_STANDARD 17)

add_executable(420J12
			   src/main.cpp
			   src/entity.cpp
			   # Si vous ajoutez d'autres fichiers .cpp, il faut les lister ici:
			   src/component.cpp
			   src/game.cpp
			   )

target_link_libraries(420J12 PRIVATE SDL3 SDL3_image SDL3_ttf)
