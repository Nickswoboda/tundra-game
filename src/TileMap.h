#pragma once

#include "Tile.h"

class TileMap
{
public:
	TileMap(const std::string& file_path, int tile_size);

	void Render();
	std::vector<std::vector<Tile>> tiles_;
	int tile_size_ = 0;
	int width_ = 0;
	int height_ = 0;

};