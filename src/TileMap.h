#pragma once

#include "Tile.h"

#include "GameObject.h"
class TileMap
{
public:
	TileMap(const std::string& file_path, int tile_size);

	void Render();
	Tile& GetTileAt(int col, int row) { return tiles_[row][col]; }
	std::vector<Tile*> GetTilesUnderneath(int x, int y, int w, int h);
	std::vector<Tile*> GetTilesUnderneath(const GameObject& obj);
	Aegis::Vec2 GetTileIndex(const Tile& tile);
	Aegis::Vec2 GetGridIndexByPos(int x, int y);
	std::vector<std::vector<Tile>> tiles_;
	int tile_size_ = 0;
	int width_ = 0;
	int height_ = 0;

	std::vector<Pellet> pellets_;

};