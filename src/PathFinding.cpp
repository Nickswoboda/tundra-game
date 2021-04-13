#include "PathFinding.h"

#include <iostream>

Aegis::Vec2 GetSlidingTargetTile(const TileMap& tile_map, const Aegis::Vec2& start, const Aegis::Vec2& dir)
{
	Aegis::Vec2 pos = start;

	//check each tile, if ice keep going. If ground, stop. If wall/end of map go back
	pos += dir;
	const Tile* tile = tile_map.GetTileByIndex(pos); 
	while (tile && tile->is_slippery_){
		pos += dir;
		tile = tile_map.GetTileByIndex(pos); 
	}
	if (!tile || tile->is_solid_){
		pos -= dir;
	}

	return pos;
}

std::vector<Aegis::Vec2> GetAllTraveledIndicesSliding(const TileMap& tile_map, const Aegis::Vec2& start, const Aegis::Vec2& dir)
{
	std::vector<Aegis::Vec2> indices;
	Aegis::Vec2 pos = start;

	//add all ice tiles in direction before hitting non-ice.
	pos += dir;
	const Tile* tile = tile_map.GetTileByIndex(pos); 
	while (tile && tile->is_slippery_){
		indices.push_back(pos);
		pos += dir;
		tile = tile_map.GetTileByIndex(pos); 
	}
	//if didnt hit wall or end of map, add ground tile
	if (tile && !tile->is_solid_){
		indices.push_back(pos);
	}
	if (indices.empty()){
		indices.push_back(start);
	}

	return indices;
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

		if (slides){
			auto traveled_sliding_neighbors = GetNeighborTilesSliding(tile_map, current);

			bool end_found = false;
			for (const auto& dir : traveled_sliding_neighbors){
				//check to see if would slide past end point
				for (int i = 0; i < dir.size() - 1; ++i){
					if (dir[i] == end){
						end_found = true;
						parent[end.x][end.y] = dir.back();
					}
				}

				auto end_index = dir.back();
				if (parent[end_index.x][end_index.y].x == -1 ) {
					frontier.push_back(end_index);
					parent[end_index.x][end_index.y] = current;
				}
			}

			if (end_found){
				break;
			}
		} else {
			auto neighbors = GetNeighborTilesMoving(tile_map, current);
			for (auto& neighbor : neighbors) {
				//if not already in list
				if (parent[neighbor.x][neighbor.y].x == -1 ) {
					frontier.push_back(neighbor);
					parent[neighbor.x][neighbor.y] = current;
				}
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

std::vector<std::vector<Aegis::Vec2>> GetNeighborTilesSliding(const TileMap& tile_map, const Aegis::Vec2& tile)
{
	std::vector<std::vector<Aegis::Vec2>> neighbors;
	neighbors.push_back(GetAllTraveledIndicesSliding(tile_map, tile, {0, -1}));
	neighbors.push_back(GetAllTraveledIndicesSliding(tile_map, tile, {0, 1}));
	neighbors.push_back(GetAllTraveledIndicesSliding(tile_map, tile, {-1, 0}));
	neighbors.push_back(GetAllTraveledIndicesSliding(tile_map, tile, {1, 0}));

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
