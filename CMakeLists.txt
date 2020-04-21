cmake_minimum_required (VERSION 3.8)

project ("Tundra")

add_subdirectory("dependencies/Aegis")
set (CMAKE_CXX_STANDARD 17)
add_executable (Tundra 
	"src/main.cpp" 
	"src/GameplayLayer.h"
	"src/GameplayLayer.cpp"
	"src/GameObject.cpp"
	"src/GameObject.h"
	"src/TileMap.h"
	"src/TileMap.cpp"
	"src/Tile.h" "src/GameplayState.cpp" "src/MenuState.cpp")

target_link_libraries(Tundra Aegis)

include_directories(
	"dependencies/Aegis/"
	)


file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
