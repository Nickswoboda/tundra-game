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
	"src/Scenes/LevelSelectScene.cpp" "src/Scenes/OptionsScene.cpp" )

target_link_libraries(Tundra Aegis)

include_directories(
	"dependencies/Aegis/"
	)

Set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
IF( EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" )
  EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
    ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
  )
ENDIF()

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
