# CMakeList.txt : CMake project for Tundra, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("Tundra")

add_subdirectory("dependencies/Aegis")

# Add source to this project's executable.
add_executable (Tundra "src/Tundra.cpp")
target_link_libraries(Tundra Aegis)
include_directories(
	"dependencies/Aegis/"
	)
# TODO: Add tests and install targets if needed.
