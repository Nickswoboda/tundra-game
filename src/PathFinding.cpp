#include "PathFinding.h"

#include <iostream>

Aegis::Vec2 GetSlidingTargetTile(const TileMap& tile_map, const Aegis::Vec2& start, const Aegis::Vec2& dir)
{
	Aegis::Vec2 pos = start;

	//Check each tile, if ice, go to next tile. If ground, stop, if wall or edge of map, go back one tile
	pos += dir;
	const Tile* tile = tile_map.GetTileByIndex(pos); 
	while (tile && tile->is_slippery_){
		pos += dir;
		tile = tile_map.GetTileByIndex(pos); 
	}
	if (!tile || tile->is_solid_){
		pos -= dir;
	}

	return Aegis::Vec2(pos.x, pos.y);
}


Aegis::Vec2 GetTargetTileCoordBFS(const TileMap& tile_map, const Aegis::Vec2& start, const Aegis::Vec2& end, bool slides)
{
	std::vector<Aegis::Vec2> frontier;
	frontier.push_back(start);

	std::vector<std::vector<Aegis::Vec2>> parent(tile_map.grid_size_.x, std::vector<Aegis::Vec2>(tile_map.grid_size_.y, Aegis::Vec2(-1, -1)));
	parent[start.x][start.y] = start;

	while (!frontier.empty()) {
		auto current = frontier[0];
		frontier.erase(frontier.begin());

		if (current == end) {
			break;
		}
		std::vector<Aegis::Vec2> neighbors = slides ? GetNeighborTilesSliding(tile_map, current) : GetNeighborTilesMoving(tile_map, current);

		for (auto& neighbor : neighbors) {
			//if not already in list
			if (parent[neighbor.x][neighbor.y].x == -1 ) {
				frontier.push_back(neighbor);
				parent[neighbor.x][neighbor.y] = current;
			}
		}
	}
	//if no path available
	if (parent[end.x][end.y].x == -1) {
		std::cout << "Unable to find path BFS";
		return start;
	}

	Aegis::Vec2 path_start = end;
	while (parent[path_start.x][path_start.y] != start) {
		path_start = parent[path_start.x][path_start.y];
	}
	return path_start;
}

std::vector<Aegis::Vec2> GetNeighborTilesSliding(const TileMap& tile_map, const Aegis::Vec2& tile)
{
	std::vector<Aegis::Vec2> neighbors;
	neighbors.push_back(GetSlidingTargetTile(tile_map, tile, {0, -1}));
	neighbors.push_back(GetSlidingTargetTile(tile_map, tile, {0, 1}));
	neighbors.push_back(GetSlidingTargetTile(tile_map, tile, {-1, 0}));
	neighbors.push_back(GetSlidingTargetTile(tile_map, tile, {1, 0}));

	return neighbors;
}

std::vector<Aegis::Vec2> GetNeighborTilesMoving(const TileMap& tile_map, const Aegis::Vec2& tile)
{
	auto adjacent_indices = tile_map.GetAdjacentTilesIndices(tile);

	std::vector<Aegis::Vec2> neighbors;
	for (auto index : adjacent_indices){
		if (!tile_map.GetTileByIndex(index)->is_solid_){
			neighbors.push_back(index);
		}
	}

	return neighbors;
}
