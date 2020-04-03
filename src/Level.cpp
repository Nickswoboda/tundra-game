#include "Level.h"

Level::Level(const std::string& file_path, Player& player)
	:player_(&player)
{
	auto temp_wall = std::make_unique<Wall>();
	temp_wall->x_pos_ = 150;

	auto temp_pellet = std::make_unique<Pellet>();
	temp_pellet->x_pos_ = 100;

	game_objects_.push_back(std::move(temp_wall));
	game_objects_.push_back(std::move(temp_pellet));

}

void Level::OnUpdate()
{
	for (auto& obj : game_objects_) {
		obj->OnUpdate();
	}
}

void Level::OnRender(float delta_time)
{
	for (auto& obj : game_objects_) {
		obj->OnRender(delta_time);
	}
}
