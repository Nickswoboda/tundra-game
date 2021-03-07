#pragma once
#include "src/Aegis.h"
#include "TileMap.h"

Aegis::Vec2 GetSlidingTargetTile(const TileMap& tile_map, const Aegis::Vec2& start, const Aegis::Vec2& dir);
Aegis::Vec2 GetTargetTileCoordBFS(const TileMap& tile_map, const Aegis::Vec2& start, const Aegis::Vec2& end, bool sliding);
std::vector<Aegis::Vec2> GetNeighborTilesMoving(const TileMap& tile_map, const Aegis::Vec2& tile);
std::vector<Aegis::Vec2> GetNeighborTilesSliding(const TileMap& tile_map, const Aegis::Vec2& tile);
