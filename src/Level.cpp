#include "Level.h"

#include <fstream>
#include <sstream>

Level::Level(const std::string& file_path, Player& player)
	:player_(&player)
{
	std::ifstream file(file_path);
	std::stringstream buffer;
	buffer << file.rdbuf();

	int x = 0;
	int y = 0;

	for (const auto& tile : buffer.str()) {
		switch (tile)
		{
		case '\n': y += 32; x = 0; break;
		case ' ': x += 32; break;
		case '0': {
			game_objects_.emplace_back(std::make_unique<Wall>(x, y));
			x += 32;
			break;
		}
		case '1': {
			
			game_objects_.emplace_back(std::make_unique<Pellet>(x + 8, y + 8));
			x += 32;
			++pellet_count_;
			break;
		}
		case '3': { player_->x_pos_ = x; player_->y_pos_ = y; x += 32; break; }
		}
	}
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
