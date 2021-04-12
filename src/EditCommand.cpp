#include "EditCommand.h"

TileEditCommand::TileEditCommand(TileEditor& editor, const Aegis::Vec2 index, const Tile& new_tile)
	:editor_(editor), index_(index), new_tile_(new_tile), prev_tile_(*(editor.tile_map_.tiles_[index.x][index.y])) 
{
}

void TileEditCommand::Execute()
{
	editor_.SetTile(index_, new_tile_);
}

void TileEditCommand::Undo()
{
	editor_.SetTile(index_, prev_tile_);
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

