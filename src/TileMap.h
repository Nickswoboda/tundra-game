#pragma once

#include "Tile.h"

#include "src/Aegis.h"
#include "GameObject.h"

#include <memory>
#include <unordered_map>

enum IceNeighborFlags
{
	None =			0,
	Top	=			1 << 0,
	Bottom =		1 << 1,
	Left =			1 << 2,
	Right =			1 << 3,
	TopLeft =		1 << 4,
	TopRight =		1 << 5,
	BottomLeft =	1 << 6,
	BottomRight =	1 << 7
};

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

	const Tile* GetTileByIndex(Aegis::Vec2 index) const;
	const Tile* GetTileByPos(Aegis::Vec2 pos) const;
	void SetTile(const Aegis::Vec2 index, int flags);
	void SetTile(const Aegis::Vec2 index, const Tile& tile);
	void SetTile(const Aegis::Vec2 index, const char token);

	int GetIceNeighborFlags(Aegis::Vec2 index) const;

	std::vector<Aegis::Vec2> GetAdjacentTilesIndices(Aegis::Vec2 index) const;
	std::vector<Aegis::Vec2> GetSurroundingTilesIndices(Aegis::Vec2 index) const;
	std::vector<std::vector<bool>> GetReachableTileIndices(Aegis::Vec2 start_index) const;

	Aegis::Vec2 GetGridIndexByPos(const Aegis::Vec2& pos) const;

	std::shared_ptr<Aegis::Texture> tile_atlas_;
	int tile_size_;
	//used for determining which ground tile to place
	std::unordered_map<char, Tile> tiles_by_token;
	std::unordered_map<int, char> tile_tokens_by_flags_;
	//stored in col major ordering to coincide with rendering customs
	std::vector<std::vector<const Tile*>> tiles_;
	std::unordered_map<SpawnPoint, Aegis::Vec2> spawn_indices_;
	std::unordered_set<Aegis::Vec2> pellet_spawn_indices_;
	std::unordered_set<Aegis::Vec2> invalid_tiles_;
	Aegis::Vec2 grid_size_;

private:
	void LoadTiles();
};
