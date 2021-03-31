#pragma once

#include "Tile.h"
#include "TileMap.h"

enum class SpawnPoint
{
	None,
	Bjorn,
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
	TileEditCommand(TileMap& tile_map, const Aegis::Vec2 index, const Tile& new_tile);

	void Execute() override;
	void Undo() override;
	
	TileMap& tile_map_;
	const Aegis::Vec2 index_;
	const Tile& new_tile_;
	const Tile& prev_tile_;
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

class PlaceFishCommand : public EditCommand
{
public:
	PlaceFishCommand(TileMap& tile_map, Aegis::Vec2 index);
	void Execute() override;
	void Undo() override;

	TileMap& tile_map_;
	Aegis::Vec2 index_;
	bool placed_ = true;
};
