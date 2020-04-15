#include "GameplayLayer.h"

#include <fstream>
#include <iostream>
#include <filesystem>
GameplayLayer::GameplayLayer()
	:player_(0, 0)
{

	auto& texmgr = Aegis::TextureManager::Instance();
	texmgr.Load("assets/textures/tundra-tile-map.png");
	LoadLevel("assets/levels/level2.txt");
}

void GameplayLayer::OnUpdate()
{
	if (player_.moving_) {
		player_.Update();
		UpdatePlayerGridPosition();
		if (player_.tile_index_ == tile_map_->GetGridIndexByPos(player_.target_pos_.x, player_.target_pos_.y)) {
			player_.moving_ = false;
			player_.rect_.pos = Aegis::Vec2(player_.tile_index_.x * tile_map_->tile_size_.x, player_.tile_index_.y * tile_map_->tile_size_.y);
			player_.vel_ = Aegis::Vec2(0, 0);
			if (queued_movement_ != -1) {
				HandlePlayerMovement(queued_movement_);
				queued_movement_ = -1;
			}
		}
	}

	for (auto i = pellets_.begin(); i != pellets_.end();) {
		if (Aegis::AABBHasCollided(player_.rect_, (*i).rect_)) {
			i = pellets_.erase(i);
			if (pellets_.size() == 0) {
				player_.moving_ = false;
				player_.vel_ = Aegis::Vec2(0, 0);
				queued_movement_ = -1;
				LoadLevel("assets/levels/level1.txt");
				return;
			}
		}
		else {
			++i;
		}
	
	}
}
void GameplayLayer::OnEvent(Aegis::Event& event)
{

	static float width = 1280;
	static float height = 720;
	auto scroll_event = dynamic_cast<Aegis::MouseScrollEvent*>(&event);
	if (scroll_event) {

	zoom_ -= scroll_event->y_offset_ * .05;
	Aegis::Renderer2D::SetProjection(width * zoom_, height*zoom_);
	}
	auto key_event = dynamic_cast<Aegis::KeyEvent*>(&event);

	if (key_event && (key_event->action_ == AE_BUTTON_PRESS || key_event->action_ == AE_BUTTON_REPEAT)) {
		if (key_event->key_ == AE_KEY_J) {
			SaveLevel();
		}
		if (key_event->key_ == AE_KEY_K) {
			SpawnPellets();
		}

		if (key_event->key_ == AE_KEY_Q ||
			key_event->key_ == AE_KEY_W ||
			key_event->key_ == AE_KEY_E) {
				auto mouse_pos = Aegis::Application::GetMousePos();
				auto tile = tile_map_->GetTileByPos(mouse_pos.x, mouse_pos.y);
		
				if (tile != nullptr) {
					auto index = tile_map_->GetTileIndex(*tile);
					
					Aegis::Vec2 tile_spawn_pos(index.x * tile_map_->tile_size_.x, index.y * tile_map_->tile_size_.y);
					switch (key_event->key_)
					{
					case AE_KEY_Q: *tile = Wall(tile_spawn_pos.x, tile_spawn_pos.y); break;
					case AE_KEY_W: *tile = Ice(tile_spawn_pos.x, tile_spawn_pos.y); break;
					case AE_KEY_E: *tile = Ground(tile_spawn_pos.x, tile_spawn_pos.y); break;
					}
				}
		}
		if (key_event->key_ == AE_KEY_UP ||
			key_event->key_ == AE_KEY_DOWN ||
			key_event->key_ == AE_KEY_LEFT ||
			key_event->key_ == AE_KEY_RIGHT) {
			if (!player_.moving_) {
				HandlePlayerMovement(key_event->key_);
			}
			else if (queued_movement_ == -1){
				queued_movement_ = key_event->key_;
			}
		}
	}
}

void GameplayLayer::HandlePlayerMovement(int key_code)
{
	Aegis::Vec2 target_tile_pos;
	switch (key_code)
	{
		case GLFW_KEY_UP: {
			for (int y_index = player_.tile_index_.y - 1; y_index >= 0; --y_index) {
				auto* tile = tile_map_->GetTileByIndex(player_.tile_index_.x, y_index);
				if (tile->type_ == Tile::Type::Ground) {
					target_tile_pos = tile->pos_;
					break;
				}
				else if (tile->type_ == Tile::Type::Wall) {
					target_tile_pos = tile->pos_ + Aegis::Vec2(0, tile_map_->tile_size_.x);
					break;
				}
			}
			break;
		}
		case GLFW_KEY_DOWN: {
			for (int y_index = player_.tile_index_.y + 1; y_index < tile_map_->grid_size_.y; ++y_index) {
				auto* tile = tile_map_->GetTileByIndex(player_.tile_index_.x, y_index);
				if (tile->type_ == Tile::Type::Ground) {
					target_tile_pos = tile->pos_;
					break;
				}
				else if (tile->type_ == Tile::Type::Wall) {
					target_tile_pos = tile->pos_ - Aegis::Vec2(0, tile_map_->tile_size_.y);
					break;
				}
			}
			break;
		}
		case GLFW_KEY_LEFT: {
			for (int x_index = player_.tile_index_.x - 1; x_index >= 0; --x_index) {
				auto* tile = tile_map_->GetTileByIndex(x_index, player_.tile_index_.y);
				if (tile->type_ == Tile::Type::Ground) {
					target_tile_pos = tile->pos_;
					break;
				}
				else if (tile->type_ == Tile::Type::Wall) {
					target_tile_pos = tile->pos_ + Aegis::Vec2(tile_map_->tile_size_.x, 0);
					break;
				}
			}
			break;
		}
		case GLFW_KEY_RIGHT: {
			for (int x_index = player_.tile_index_.x + 1; x_index < tile_map_->grid_size_.x; ++x_index) {
				auto* tile = tile_map_->GetTileByIndex(x_index, player_.tile_index_.y);
				if (tile->type_ == Tile::Type::Ground) {
					target_tile_pos = tile->pos_;
					break;
				}
				else if (tile->type_ == Tile::Type::Wall) {
					target_tile_pos = tile->pos_ - Aegis::Vec2(tile_map_->tile_size_.x, 0);
					break;
				}
			}
			break;
		}
	}

	if (player_.rect_.pos == target_tile_pos) {
		return;
	}
	
	player_.target_pos_ = target_tile_pos;
	player_.moving_ = true;
	switch (key_code)
	{
	case GLFW_KEY_UP: player_.vel_.y = -player_.acceleration_; break;
	case GLFW_KEY_DOWN: player_.vel_.y = player_.acceleration_; break;
	case GLFW_KEY_LEFT: player_.vel_.x = -player_.acceleration_; break;
	case GLFW_KEY_RIGHT: player_.vel_.x = player_.acceleration_; break;
	}
}

void GameplayLayer::UpdatePlayerGridPosition()
{
	if (player_.vel_.x > 0 || player_.vel_.y > 0) {
		player_.tile_index_ = tile_map_->GetGridIndexByPos(player_.rect_.pos.x, player_.rect_.pos.y);
	}
	else if (player_.vel_.x < 0) {
		player_.tile_index_ = tile_map_->GetGridIndexByPos(player_.rect_.pos.x + tile_map_->tile_size_.x, player_.rect_.pos.y);
	}
	else if (player_.vel_.y < 0) {
		player_.tile_index_ = tile_map_->GetGridIndexByPos(player_.rect_.pos.x, player_.rect_.pos.y + tile_map_->tile_size_.y);
	}
}

void GameplayLayer::OnRender(float delta_time)
{
	Aegis::RendererClear();
	tile_map_->Render();
	player_.Render(delta_time);
	Aegis::DrawText(std::to_string(Aegis::Application::GetFrameTime()), { 0, 0 }, { 1.0f, 1.0f, 1.0f, 1.0f });
	for (auto& pellet : pellets_)
	{
		pellet.Render(0.0f);
	}
}

void GameplayLayer::SaveLevel()
{
	int level = 1;
	std::string new_file_path = "C:/dev/tundra/assets/levels/level" + std::to_string(level) + ".txt";

	while (std::filesystem::exists(new_file_path)) {
		++level;
		new_file_path = "C:/dev/tundra/assets/levels/level" + std::to_string(level) + ".txt";
	}
	std::ofstream file(new_file_path);
	
	for (int row = 0; row < tile_map_->grid_size_.x; ++row) {
		for (int col = 0; col < tile_map_->grid_size_.y; ++col) {
			auto type = tile_map_->tiles_[col][row].type_;

			switch (type)
			{
			case Tile::Type::Wall: {file << '0'; break; }
			case Tile::Type::Ice: {file << '1'; break; }
			case Tile::Type::Ground: {file << ' '; break; }
			}
		}

		//don't print newline on last row
		if (row != tile_map_->grid_size_.y - 1) {
			file << '\n';
		}
	}

	file.close();
}

void GameplayLayer::SpawnPellets()
{
	for (const auto& col : tile_map_->tiles_) {
		for (const auto& tile : col) {
			if (tile.type_ == Tile::Type::Ice) {
				pellets_.emplace_back(Pellet(tile.pos_.x + 12, tile.pos_.y + 12));
			}
		}
	}
}

void GameplayLayer::LoadLevel(const std::string& file_path)
{
	tile_map_ = std::make_unique<TileMap>(file_path, 32);

	player_.tile_index_ = tile_map_->player_start_pos;
	player_.rect_.pos = Aegis::Vec2(player_.tile_index_.x * tile_map_->tile_size_.x, player_.tile_index_.y * tile_map_->tile_size_.y);
	SpawnPellets();
}
