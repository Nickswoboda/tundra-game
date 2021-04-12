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

int GetSimplifiedFlags(int flags){
	if (flags & Top){
		flags &= ~TopLeft;
		flags &= ~TopRight;
	}
	if (flags & Bottom){
		flags &= ~BottomLeft;
		flags &= ~BottomRight;
	}
	if (flags & Left){
		flags &= ~TopLeft;
		flags &= ~BottomLeft;
	}
	if (flags & Right){
		flags &= ~TopRight;
		flags &= ~BottomRight;
	}
	
	return flags;
}

void InvalidateSurroundingIceTiles(TileMap& tile_map, Aegis::Vec2 index)
{
	auto surrounding = tile_map.GetSurroundingTilesIndices(index);
	for (const auto& idx : surrounding){
		if (tile_map.GetTileByIndex(idx)->is_slippery_){
			tile_map.invalid_tiles_.insert(idx);
		}
	}
}
void UpdateTile(TileMap& tile_map, Aegis::Vec2 index)
{
	if (!tile_map.GetTileByIndex(index)->is_solid_) return;
	
	auto flags = GetSimplifiedFlags(tile_map.GetIceNeighborFlags(index));
	if (!tile_map.tile_tokens_by_flags_.count(flags)){
		return;
	}

	tile_map.SetTile(index, flags);
}

void TileEditCommand::UpdateSurroundingWallTiles()
{
	std::array<int, 3> dir{-1, 0, 1};
	
	for (auto x = 0; x < dir.size(); ++x){
		for (auto y = 0; y < dir.size(); ++y){
			UpdateTile(tile_map_, index_ + Aegis::Vec2(dir[x], dir[y]));
		}
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

