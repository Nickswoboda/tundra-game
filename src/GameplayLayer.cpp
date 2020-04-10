#include "GameplayLayer.h"

#include <fstream>
GameplayLayer::GameplayLayer()
	:player_(270, 660), tile_map_("assets/levels/custom.txt", 32)
{}
void GameplayLayer::ResolveCollision(GameObject& obj_1, const Tile& tile)
{
	if (obj_1.vel_.x > 0) {
		obj_1.rect_.pos.x = tile.pos_.x - obj_1.rect_.size.x;
	}
	else if (obj_1.vel_.x < 0) {
		obj_1.rect_.pos.x = tile.pos_.x + tile_map_.tile_size_;
	}
	else if (obj_1.vel_.y > 0) {
		obj_1.rect_.pos.y = tile.pos_.y - obj_1.rect_.size.y;
	}
	else if (obj_1.vel_.y < 0) {
		obj_1.rect_.pos.y = tile.pos_.y + tile_map_.tile_size_;
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
		if (Aegis::AABBHasCollided(player_.rect_, (*i).rect_)) {
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
				auto tile = tile_map_.GetTileByPos(mouse_pos.x, mouse_pos.y);

				if (tile != nullptr) {
					auto index = tile_map_.GetTileIndex(*tile);

					switch (key_event->key_)
					{
					case GLFW_KEY_Q: tile_map_.tiles_[index.x][index.y] = Wall(index.y * tile_map_.tile_size_, index.x * tile_map_.tile_size_); break;
					case GLFW_KEY_W: tile_map_.tiles_[index.x][index.y] = Ice(index.y * tile_map_.tile_size_, index.x * tile_map_.tile_size_); break;
					case GLFW_KEY_E: tile_map_.tiles_[index.x][index.y] = Ground(index.y * tile_map_.tile_size_, index.x * tile_map_.tile_size_); break;
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
