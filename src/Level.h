#pragma once

#include "GameObject.h"
#include "TileMap.h"

class Level
{
public:
	Level(const std::string& file_path, Player& player);
	void Update();
	void Render(float delta_time);

	std::vector<std::unique_ptr<GameObject>> game_objects_;
	Player* player_;
	int pellet_count_ = 0;
	TileMap tile_map_;
};