#pragma once

#include "Tile.h"

#include "src/Aegis.h"
#include "GameObject.h"

#include <memory>
class TileMap
{
public:
	TileMap(const std::string& file_path, int tile_size);

	void Render();
	Tile* GetTileByIndex(int col, int row);
	Tile* GetTileByPos(int x_pos, int y_pos);
	std::vector<Tile*> GetTilesUnderneath(int x, int y, int w, int h);
	std::vector<Tile*> GetTilesUnderneath(const Aegis::AABB& rect);
	void SetTextureAtlas(const Aegis::Texture& atlas);
	Aegis::Vec2 GetTileIndex(const Tile& tile);
	Aegis::Vec2 GetGridIndexByPos(int x, int y);
	std::vector<std::vector<Tile>> tiles_;
	int tile_size_ = 0;
	int width_ = 0;
	int height_ = 0;

	std::unique_ptr<Aegis::Texture> tile_atlas_;

};