#include "TileMap.h"

#include <fstream>
#include <sstream>

TileMap::TileMap(const std::string& file_path, int tile_size)
	: tile_size_(tile_size)
{
	std::ifstream file(file_path);
	std::stringstream buffer;
	buffer << file.rdbuf();

	int x = 0;
	int y = 0;
	tiles_.push_back(std::vector<Tile>());


	for (const auto& ch : buffer.str()) {
		switch (ch)
		{
			case '\n': {
				tiles_.push_back(std::vector<Tile>());
				++y; x = 0; break;
			}
			case '0': {
				tiles_[y].emplace_back(Wall()); break;
			}
			case '1': {
				tiles_[y].emplace_back(Ice()); break;
			}
			case ' ': {
				tiles_[y].emplace_back(Ground()); break;
			}
		}
	}
	height_ = tiles_.size();
	width_ = tiles_[height_ - 1].size();
}

void TileMap::Render()
{

	for (int row = 0; row < tiles_.size(); ++row) {
		for (int col = 0; col < tiles_[row].size(); ++col) {
			Aegis::Renderer2D::DrawQuad({ col * tile_size_, row * tile_size_}, { tile_size_, tile_size_ }, tiles_[row][col].color_);
		}
	}
}
