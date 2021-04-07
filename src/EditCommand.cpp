#include "EditCommand.h"

TileEditCommand::TileEditCommand(TileMap& tile_map, const Aegis::Vec2 index, const Tile& new_tile)
	:tile_map_(tile_map), index_(index), new_tile_(new_tile), prev_tile_(*(tile_map_.tiles_[index.x][index.y])) 
{
}

void TileEditCommand::Execute()
{
	tile_map_.SetTile(index_, new_tile_);
	UpdateSurroundingWallTiles();
}
void TileEditCommand::Undo()
{
	tile_map_.SetTile(index_, prev_tile_);
	UpdateSurroundingWallTiles();
}

void UpdateGroundTile(TileMap& tile_map, Aegis::Vec2 index)
{
	if (!(tile_map.GetTileByIndex(index.x, index.y)->is_solid_)){
		return;
	}
	auto top = tile_map.GetTileByIndex(index.x, index.y - 1);
	auto bot = tile_map.GetTileByIndex(index.x, index.y + 1);
	auto left = tile_map.GetTileByIndex(index.x - 1, index.y);
	auto right = tile_map.GetTileByIndex(index.x + 1, index.y);

	if (top && !top->is_solid_){
		if (left && !left->is_solid_){
			tile_map.SetTile(index, '8');
			return;
		} else if (right && !right->is_solid_){
			tile_map.SetTile(index, '9');
			return;
		} else {
			tile_map.SetTile(index, '6');
			return;
		}
	} else if (bot && !bot->is_solid_){
		if (left && !left->is_solid_){
			tile_map.SetTile(index, 'a');
			return;
		} else if (right && !right->is_solid_){
			tile_map.SetTile(index, 'b');
			return;
		} else {
			tile_map.SetTile(index, '1');
			return;
		}
	}  else if (right && !right->is_solid_){
			tile_map.SetTile(index, '3');
			return;
	} else if (left && !left->is_solid_){
			tile_map.SetTile(index, '4');
			return;
	} else {
		auto top_left = tile_map.GetTileByIndex(index.x - 1, index.y - 1);
		if (top_left && !top_left->is_solid_){
			tile_map.SetTile(index, '7');
			return;
		}
		auto top_right = tile_map.GetTileByIndex(index.x + 1, index.y - 1);
		if (top_right && !top_right->is_solid_){
			tile_map.SetTile(index, '5');
			return;
		}
		auto bot_left = tile_map.GetTileByIndex(index.x - 1, index.y + 1);
		if (bot_left && !bot_left->is_solid_){
			tile_map.SetTile(index, '2');
			return;
		}
		auto bot_right = tile_map.GetTileByIndex(index.x + 1, index.y + 1);
		if (bot_right && !bot_right->is_solid_){
			tile_map.SetTile(index, '0');
			return;
		}
	}

	tile_map.SetTile(index, 'w');


}
void TileEditCommand::UpdateSurroundingWallTiles()
{
	UpdateGroundTile(tile_map_, index_);
	for (auto& tile : tile_map_.GetDiagonalTilesIndices(index_)){
		UpdateGroundTile(tile_map_, tile);
	}
	for (auto& tile : tile_map_.GetAdjacentTilesIndices(index_)){
		UpdateGroundTile(tile_map_, tile);
	}
}

SpawnEditCommand::SpawnEditCommand(TileMap& tile_map, SpawnPoint spawn_point, Aegis::Vec2 new_index)
	: tile_map_(tile_map), spawn_point_(spawn_point),  new_index_(new_index) 
{
	prev_index_ = tile_map_.spawn_indices_[spawn_point_];
}

void SpawnEditCommand::Execute()
{
	tile_map_.spawn_indices_[spawn_point_] = new_index_;
}

void SpawnEditCommand::Undo()
{
	tile_map_.spawn_indices_[spawn_point_] = prev_index_;
}

FishEditCommand::FishEditCommand(TileMap& tile_map, Aegis::Vec2 index)
	:tile_map_(tile_map), index_(index)
{ }

void FishEditCommand::Execute()
{
	if (tile_map_.pellet_spawn_indices_.count(index_)){
		tile_map_.pellet_spawn_indices_.erase(index_);
		placed_ = false;
	} else {
		tile_map_.pellet_spawn_indices_.emplace(index_);
	}
}

void FishEditCommand::Undo()
{
	if (placed_){
		tile_map_.pellet_spawn_indices_.erase(index_);
	} else {
		tile_map_.pellet_spawn_indices_.emplace(index_);
	}
}

