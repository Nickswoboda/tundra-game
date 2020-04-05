#pragma once

#include "Tile.h"

class TileMap
{
public:
	TileMap(const std::string& file_path);

	void Render();
	std::vector<Tile> tiles_;

};