#pragma once

#include "GameObject.h"

class Level
{
public:
	Level(const std::string& file_path, Player& player);
	void OnUpdate();
	void OnRender(float delta_time);

	std::vector<std::unique_ptr<GameObject>> game_objects_;
	Player* player_;
};