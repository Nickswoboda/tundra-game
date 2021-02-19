cmake_minimum_required (VERSION 3.8)
set (CMAKE_CXX_STANDARD 17)

project ("Tundra")

add_executable (Tundra 
	"src/main.cpp" 
	"src/TileMap.cpp"
	"src/Particle.cpp"
	"src/GameObject.cpp"
	"src/EditCommand.cpp"
	"src/PauseMenu.cpp"
	"src/Scenes/MenuScene.cpp"
	"src/Scenes/OptionsScene.cpp" 
	"src/Scenes/GameplayScene.cpp" 
	"src/Scenes/LevelEditorScene.cpp"
	"src/Scenes/LevelSelectScene.cpp") 


add_subdirectory("dependencies/Aegis")
target_link_libraries(Tundra Aegis)
target_include_directories(Tundra PUBLIC "dependencies/Aegis/")

Set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
IF( EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" )
  EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
    ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
  )
ENDIF()

add_custom_command(TARGET Tundra PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:Tundra>/assets)
