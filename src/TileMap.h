#pragma once

#include "Tile.h"

#include "src/Aegis.h"
#include "GameObject.h"

#include <memory>
class TileMap
{
public:

	TileMap(const std::string& file_path, int tile_size, std::shared_ptr<Aegis::Texture> atlas);
	//base TileMap for level editor
	TileMap(int width, int height, int tile_size, std::shared_ptr<Aegis::Texture> atlas);

	void Save();

	void Render() const;

	const Tile& GetTileByIndex(int col, int row);
	const Tile& GetTileByPos(int x_pos, int y_pos);

	std::vector<Aegis::Vec2> GetAdjacentTilesIndices(Aegis::Vec2 index);
	std::vector<Tile*> GetTilesUnderneath(int x, int y, int w, int h);
	std::vector<Tile*> GetTilesUnderneath(const Aegis::AABB& rect);
	//TODO: possibly create hash func for Aegis::Vec2
	std::vector<std::vector<bool>> GetReachableTileIndices(Aegis::Vec2 start_index);

	Aegis::Vec2 GetGridIndexByPos(const Aegis::Vec2& pos) const;
	std::shared_ptr<Aegis::Texture> tile_atlas_;
	int tile_size_;
	Tile wall_tile_;
	Tile ice_tile_;
	Tile ground_tile_;
	std::vector<std::vector<Tile*>> tiles_;
	Aegis::Vec2 bruce_spawn_index_;
	Aegis::Vec2 bjorn_spawn_index_;
	Aegis::Vec2 brutus_spawn_index_;
	Aegis::Vec2 grid_size_;
};
