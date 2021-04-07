#pragma once

#include "Tile.h"

#include "src/Aegis.h"
#include "GameObject.h"

#include <memory>
#include <unordered_map>

enum class SpawnPoint
{
	None,
	Bjorn,
	Brutus,
	Bruce
};

class TileMap
{
public:

	TileMap(const std::string& file_path, int tile_size, std::shared_ptr<Aegis::Texture> atlas);
	//base TileMap for level editor
	TileMap(int width, int height, int tile_size, std::shared_ptr<Aegis::Texture> atlas);

	void Save(int level_num = -1);

	void Render() const;
	void DrawGridLines() const;
	void Clear();

	const Tile* GetTileByIndex(int col, int row) const;
	const Tile* GetTileByPos(int x_pos, int y_pos) const;
	void SetTile(const Aegis::Vec2 index, const Tile& tile);
	void SetTile(const Aegis::Vec2 index, const char token);
	//used for updating 
	std::vector<Aegis::Vec2> GetDiagonalTilesIndices(Aegis::Vec2 index) const;
	std::vector<Aegis::Vec2> GetAdjacentTilesIndices(Aegis::Vec2 index) const;
	std::vector<const Tile*> GetTilesUnderneath(int x, int y, int w, int h) const;
	std::vector<const Tile*> GetTilesUnderneath(const Aegis::AABB& rect) const;
	//TODO: possibly create hash func for Aegis::Vec2
	std::vector<std::vector<bool>> GetReachableTileIndices(Aegis::Vec2 start_index) const;

	Aegis::Vec2 GetGridIndexByPos(const Aegis::Vec2& pos) const;

	std::shared_ptr<Aegis::Texture> tile_atlas_;
	int tile_size_;
	std::unordered_map<char, Tile> tiles_map_;
	//stored in col major ordering to coincide with rendering customs
	std::vector<std::vector<const Tile*>> tiles_;
	std::unordered_map<SpawnPoint, Aegis::Vec2> spawn_indices_;
	std::unordered_set<Aegis::Vec2> pellet_spawn_indices_;
	Aegis::Vec2 grid_size_;

private:
	void LoadTiles();
};
