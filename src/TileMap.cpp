#include "TileMap.h"

#include <fstream>
#include <sstream>

TileMap::TileMap(const std::string& file_path)
{
	std::ifstream file(file_path);
	std::stringstream buffer;
	buffer << file.rdbuf();

	int x = 0;
	int y = 0;

	for (const auto& ch : buffer.str()) {
		switch (ch)
		{
		case '\n': {
			y += 32; x = 0; break;
		}
		case '0': {
			tiles_.emplace_back(Wall(x, y)); x += 32; break;
		}
		case '1': {
			tiles_.emplace_back(Ice(x, y)); x += 32;  break;
		}
		case ' ': {
			tiles_.emplace_back(Ground(x, y)); x += 32;  break;
		}
		}
	}
}

void TileMap::Render()
{
	for (const auto& tile : tiles_) {
		Aegis::Renderer2D::DrawQuad(tile.pos_, tile.size_, tile.color_);
	}
}
