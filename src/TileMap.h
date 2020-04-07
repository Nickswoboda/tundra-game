#pragma once

#include "Tile.h"

#include "GameObject.h"
class TileMap
{
public:
	TileMap(const std::string& file_path, int tile_size);

	void Render();
	Tile& GetTileAt(int col, int row) { return tiles_[row][col]; }
	std::vector<Tile*> GetTilesUnderneath(const GameObject& obj);
	std::vector<std::vector<Tile>> tiles_;
	int tile_size_ = 0;
	int width_ = 0;
	int height_ = 0;

};