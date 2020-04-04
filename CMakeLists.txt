# CMakeList.txt : CMake project for Tundra, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("Tundra")

add_subdirectory("dependencies/Aegis")

# Add source to this project's executable.
add_executable (Tundra 
	"src/main.cpp" 
	"src/GameplayLayer.h"
	"src/GameplayLayer.cpp"
	"src/GameObject.cpp"
	"src/GameObject.h"
	"src/Level.h"
	"src/Level.cpp"
	)

target_link_libraries(Tundra Aegis)

include_directories(
	"dependencies/Aegis/"
	)


file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
