#include "Level.h"

#include <fstream>
#include <sstream>

Level::Level(const std::string& file_path, Player& player)
	:player_(&player), tile_map_(file_path)
{
	player_->x_pos_ = 32;
	player_->y_pos_ = 32;
}

void Level::Update()
{
	for (auto& obj : game_objects_) {
		obj->Update();
	}
}

void Level::Render(float delta_time)
{
	tile_map_.Render();
	for (auto& obj : game_objects_) {
		obj->Render(delta_time);
	}
}
