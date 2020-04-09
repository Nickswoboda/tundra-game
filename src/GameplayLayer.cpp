#include "GameplayLayer.h"

#include <fstream>
GameplayLayer::GameplayLayer()
	:player_(270, 660), tile_map_("assets/levels/custom.txt", 20)
{}
bool GameplayLayer::HasCollided(const GameObject& obj_1, const GameObject& obj_2)
{
	if ((int)obj_1.pos_.x < obj_2.pos_.x + obj_2.size_.x &&
		(int)obj_1.pos_.x + obj_1.size_.x > obj_2.pos_.x &&
		(int)obj_1.pos_.y < obj_2.pos_.y + obj_2.size_.y &&
		(int)obj_1.pos_.y + obj_1.size_.y > obj_2.pos_.y) {

		std::cout << "Collision Detected\n";
		return true;
	}

	return false;
}
void GameplayLayer::ResolveCollision(GameObject& obj_1, const Tile& tile)
{
	if (obj_1.vel_.x > 0) {
		obj_1.pos_.x = tile.pos_.x - obj_1.size_.x;
	}
	else if (obj_1.vel_.x < 0) {
		obj_1.pos_.x = tile.pos_.x + tile_map_.tile_size_;
	}
	else if (obj_1.vel_.y > 0) {
		obj_1.pos_.y = tile.pos_.y - obj_1.size_.y;
	}
	else if (obj_1.vel_.y < 0) {
		obj_1.pos_.y = tile.pos_.y + tile_map_.tile_size_;
	}
	obj_1.vel_.x = 0;
	obj_1.vel_.y = 0;
}
void GameplayLayer::OnUpdate()
{
	player_.Update();
	
	auto tiles = tile_map_.GetTilesUnderneath(player_);
	
	for (auto tile : tiles) {
		if (tile->is_solid_) {
			ResolveCollision(player_, *tile);
		}
	}

	for (auto i = tile_map_.pellets_.begin(); i != tile_map_.pellets_.end();) {
		if (HasCollided(player_, *i)) {
			i = tile_map_.pellets_.erase(i);
		}
		else {
			++i;
		}

	}
}
void GameplayLayer::OnEvent(Aegis::Event& event)
{
	auto key_event = dynamic_cast<Aegis::KeyEvent*>(&event);

	if (key_event && key_event->action_ == GLFW_PRESS) {
		if (key_event->key_ == GLFW_KEY_J) {
			SaveLevel();
		}
		
		if (key_event->key_ == GLFW_KEY_Q ||
			key_event->key_ == GLFW_KEY_W ||
			key_event->key_ == GLFW_KEY_E) {
				auto mouse_pos = Aegis::Application::GetMousePos();
				auto tile = tile_map_.GetTilesUnderneath(mouse_pos.x, mouse_pos.y, 1, 1);


				if (tile.size() > 0) {
					auto single_tile = tile[0];
					auto index = tile_map_.GetTileIndex(*single_tile);

					switch (key_event->key_)
					{
					case GLFW_KEY_Q: tile_map_.tiles_[index.y][index.x] = Wall(index.x * tile_map_.tile_size_, index.y * tile_map_.tile_size_); break;
					case GLFW_KEY_W: tile_map_.tiles_[index.y][index.x] = Ice(index.x * tile_map_.tile_size_, index.y * tile_map_.tile_size_); break;
					case GLFW_KEY_E: tile_map_.tiles_[index.y][index.x] = Ground(index.x * tile_map_.tile_size_, index.y * tile_map_.tile_size_); break;
					default:
						break;
					}
				}
		}
	}

	player_.OnEvent(event);
}

void GameplayLayer::OnRender(float delta_time)
{
	Aegis::RendererClear();
	tile_map_.Render();
	player_.Render(delta_time);
	Aegis::DrawText(std::to_string(Aegis::Application::GetFrameTime()), { 0, 0 }, { 1.0f, 1.0f, 1.0f, 1.0f });
}

void GameplayLayer::SaveLevel()
{
	std::ofstream file("C:/dev/tundra/assets/levels/custom.txt");
	
	for (int row = 0; row < tile_map_.height_; ++row) {
		for (int col = 0; col < tile_map_.width_; ++col) {
			auto type = tile_map_.tiles_[row][col].type_;

			switch (type)
			{
			case Tile::Type::Wall: {file << '0'; break; }
			case Tile::Type::Ice: {file << '1'; break; }
			case Tile::Type::Ground: {file << ' '; break; }
			}
		}

		//don't print newline on last row
		if (row != tile_map_.height_ - 1) {
			file << '\n';
		}
	}

	file.close();
}
