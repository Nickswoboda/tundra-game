#pragma once

#include "Tile.h"
#include "TileMap.h"

enum class SpawnPoint
{
	None,
	Bjorne,
	Brutus,
	Bruce
};

class EditCommand
{
public:
	virtual ~EditCommand() {}
	virtual void Execute() = 0; 
	virtual void Undo() = 0; 
};

class TileEditCommand : public EditCommand
{
public:
	TileEditCommand(Tile& tile, Tile::Type new_type);

	void Execute() override;
	void Undo() override;
	
	Tile& tile_;
	Tile::Type prev_type_;
	Tile::Type new_type_;
};

class SpawnEditCommand : public EditCommand
{
public:
	SpawnEditCommand(TileMap& tile_map, SpawnPoint spawn_point, Aegis::Vec2 new_index);

	void Execute() override;
	void Undo() override;

	TileMap& tile_map_;
	SpawnPoint spawn_point_;
	Aegis::Vec2 prev_index_;
	Aegis::Vec2 new_index_;
};
