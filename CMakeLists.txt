cmake_minimum_required (VERSION 3.8)

project ("Tundra")

add_subdirectory("dependencies/Aegis")
set (CMAKE_CXX_STANDARD 17)
add_executable (Tundra 
	"src/main.cpp" 
	"src/GameObject.cpp"
	"src/GameObject.h"
	"src/TileMap.h"
	"src/TileMap.cpp"
	"src/Tile.h" "src/Scenes/GameplayScene.cpp" "src/Scenes/MenuScene.cpp"
	"src/FontManager.h" "src/Button.cpp" "src/Scenes/LevelSelectScene.cpp")

target_link_libraries(Tundra Aegis)

include_directories(
	"dependencies/Aegis/"
	)


file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
