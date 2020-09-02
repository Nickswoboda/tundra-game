#pragma once

#include "Tile.h"

#include "src/Aegis.h"
#include "GameObject.h"

#include <memory>
class TileMap
{
public:

	TileMap(const std::string& file_path, int tile_size);
	//base TileMap for level editor
	TileMap(int width, int height, int tile_size);

	void Save();

	void SetTextureAtlas(const Aegis::Texture& atlas);
	void Render() const;

	Tile* GetTileByIndex(int col, int row);
	Tile* GetTileByPos(int x_pos, int y_pos);

	std::vector<Tile*> GetAdjacentTiles(Aegis::Vec2 index);
	std::vector<Tile*> GetTilesUnderneath(int x, int y, int w, int h);
	std::vector<Tile*> GetTilesUnderneath(const Aegis::AABB& rect);
	//TODO: possibly create hash func for Aegis::Vec2
	std::vector<std::vector<bool>> GetReachableTileIndices(Aegis::Vec2 start_index);

	Aegis::Vec2 GetTileIndex(const Tile& tile) const;
	Aegis::Vec2 GetGridIndexByPos(const Aegis::Vec2& pos) const;

	std::vector<std::vector<Tile>> tiles_;
	std::shared_ptr<Aegis::Texture> tile_atlas_;
	std::vector<std::shared_ptr<Aegis::SubTexture>> tile_textures_;
	Aegis::Vec2 bruce_spawn_index_;
	Aegis::Vec2 bjorn_spawn_index_;
	Aegis::Vec2 brutus_spawn_index_;
	Aegis::Vec2 tile_size_;
	Aegis::Vec2 grid_size_;




};
