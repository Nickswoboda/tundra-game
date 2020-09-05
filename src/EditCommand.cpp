#include "EditCommand.h"

TileEditCommand::TileEditCommand(const TileMap& tile_map, const Aegis::Vec2 index, const Tile& new_tile)
	:tile_map_(tile_map), index_(index), new_tile_(new_tile), prev_tile_(tile_map_.tiles_[index.x][index.y]) 
{
}

void TileEditCommand::Execute()
{
	switch (new_type_){
		case Tile::Ground: tile_ = Ground(tile_.pos_.x, tile_.pos_.y); break;
		case Tile::Ice: tile_ = Ice(tile_.pos_.x, tile_.pos_.y); break;
		case Tile::Wall: tile_ = Wall(tile_.pos_.x, tile_.pos_.y); break;
		case Tile::NumTypes: break;
	}
}
void TileEditCommand::Undo()
{
	switch (prev_type_){
		case Tile::Ground: tile_ = Ground(tile_.pos_.x, tile_.pos_.y); break;
		case Tile::Ice: tile_ = Ice(tile_.pos_.x, tile_.pos_.y); break;
		case Tile::Wall: tile_ = Wall(tile_.pos_.x, tile_.pos_.y); break;
		case Tile::NumTypes: break;
	}
}

SpawnEditCommand::SpawnEditCommand(TileMap& tile_map, SpawnPoint spawn_point, Aegis::Vec2 new_index)
	: tile_map_(tile_map), spawn_point_(spawn_point),  new_index_(new_index) 
{
	switch (spawn_point_){
		case SpawnPoint::Bjorn: prev_index_ = tile_map_.bjorn_spawn_index_; break;
		case SpawnPoint::Brutus: prev_index_ = tile_map_.brutus_spawn_index_; break;
		case SpawnPoint::Bruce: prev_index_ = tile_map_.bruce_spawn_index_; break;
		case SpawnPoint::None: break;
	}
}

void SpawnEditCommand::Execute()
{
	switch (spawn_point_){
		case SpawnPoint::Bjorn: tile_map_.bjorn_spawn_index_ = new_index_; break;
		case SpawnPoint::Brutus: tile_map_.brutus_spawn_index_ = new_index_; break;
		case SpawnPoint::Bruce:  tile_map_.bruce_spawn_index_ = new_index_; break;
		case SpawnPoint::None: break;
	}

}

void SpawnEditCommand::Undo()
{
	switch (spawn_point_){
		case SpawnPoint::Bjorn: tile_map_.bjorn_spawn_index_ = prev_index_; break;
		case SpawnPoint::Brutus: tile_map_.brutus_spawn_index_ = prev_index_; break;
		case SpawnPoint::Bruce:  tile_map_.bruce_spawn_index_ = prev_index_; break;
		case SpawnPoint::None: break;
	}

}
