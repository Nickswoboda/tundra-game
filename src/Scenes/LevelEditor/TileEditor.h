#pragma once

#include "../../TileMap.h"

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

class TileEditor
{
public:
	TileEditor(TileMap& tile_map);

	void CalculateIceNeighborFlags();
	void DrawGridLines() const;
	int GetIceNeighborFlags(Aegis::Vec2 index) const;

	void UpdateSurroundingWallTiles(Aegis::Vec2 index);
	void UpdateTile(Aegis::Vec2 index);
	void SetTile(Aegis::Vec2 index, const Tile& tile);

	void Save(int level_num);

	TileMap& tile_map_;
	std::unordered_map<int, char> tile_tokens_by_flags_;
	std::vector<std::vector<int>> ice_flags_;
};


