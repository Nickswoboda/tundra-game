#include "GameplayLayer.h"

#include <fstream>
#include <iostream>
#include <filesystem>
GameplayLayer::GameplayLayer()
	:player_(0, 0), enemy_(0, 0), camera_(0, 1280, 720, 0)
{
	auto& texmgr = Aegis::TextureManager::Instance();
	texmgr.Load("assets/textures/tundra-tile-map.png");
	LoadLevel("assets/levels/level2.txt");
}

void GameplayLayer::OnUpdate()
{
	if (player_.moving_) {
		player_.Update();
		UpdateGridPosition(player_);
		if (player_.tile_index_ == tile_map_->GetGridIndexByPos(player_.target_pos_.x, player_.target_pos_.y)) {
			player_.moving_ = false;
			player_.SetPosition(player_.tile_index_ * tile_map_->tile_size_);
			player_.vel_ = Aegis::Vec2(0, 0);
			if (queued_movement_ != -1) {
				HandlePlayerMovement(queued_movement_);
				queued_movement_ = -1;
			}
		}
	}
	if (enemy_.moving_) {
		UpdateGridPosition(enemy_);

		if (enemy_.tile_index_ == tile_map_->GetGridIndexByPos(enemy_.target_pos_.x, enemy_.target_pos_.y)) {
			enemy_.moving_ = false;
			enemy_.SetPosition(enemy_.tile_index_ * tile_map_->tile_size_);
			enemy_.vel_ = Aegis::Vec2(0, 0);
			enemy_.target_pos_ = GetEnemyTargetPos();
		}
	}
	enemy_.Update();

	if (Aegis::AABBHasCollided(player_.rect_, enemy_.rect_)) {
		SetObjectOnGrid(player_, tile_map_->player_start_pos_);
		player_.moving_ = false;
		player_.vel_ = Aegis::Vec2(0, 0);
		queued_movement_ = -1;
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
	auto key_event = dynamic_cast<Aegis::KeyEvent*>(&event);

	if (key_event && (key_event->action_ == AE_BUTTON_PRESS || key_event->action_ == AE_BUTTON_REPEAT)) {
		if (key_event->key_ == AE_KEY_J) {
			SaveLevel();
		}
		if (key_event->key_ == AE_KEY_K) {
			ResetLevel();
		}

		static float x = 0;
		static float y = 0;
		if (key_event->key_ == AE_KEY_W) {
			--y;
		}
		if (key_event->key_ == AE_KEY_S) {
			++y;
		}
		if (key_event->key_ == AE_KEY_A) {
			--x;
		}
		if (key_event->key_ == AE_KEY_D) {
			++x;
		}
		camera_.SetPosition({ x, y, 1.0 });

		if (key_event->key_ == AE_KEY_R ||
			key_event->key_ == AE_KEY_T ||
			key_event->key_ == AE_KEY_Y) {
				auto mouse_pos = Aegis::Application::GetMousePos();
				auto tile = tile_map_->GetTileByPos(mouse_pos.x, mouse_pos.y);
		
				if (tile != nullptr) {
					auto index = tile_map_->GetTileIndex(*tile);
					
					Aegis::Vec2 tile_spawn_pos = index * tile_map_->tile_size_;
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
	Direction dir = Direction::Up;
	switch (key_code)
	{
	case GLFW_KEY_UP: dir = Direction::Up;  break;
	case GLFW_KEY_DOWN: dir = Direction::Down;  break;
	case GLFW_KEY_LEFT: dir = Direction::Left;  break;
	case GLFW_KEY_RIGHT: dir = Direction::Right;  break;
	}

	Aegis::Vec2 target_tile_pos = GetTargetTile(player_, dir);
	

	if (player_.rect_.pos == target_tile_pos) {
		return;
	}
	
	player_.target_pos_ = target_tile_pos;
	player_.moving_ = true;
	player_.vel_ = (target_tile_pos - player_.rect_.pos).Normalized();
}

void GameplayLayer::UpdateGridPosition(GameObject& obj)
{
	if (obj.vel_.x > 0 || obj.vel_.y > 0) {
		obj.tile_index_ = tile_map_->GetGridIndexByPos(obj.rect_.pos.x, obj.rect_.pos.y);
	}
	else if (obj.vel_.x < 0) {
		obj.tile_index_ = tile_map_->GetGridIndexByPos(obj.rect_.pos.x + tile_map_->tile_size_.x, obj.rect_.pos.y);
	}
	else if (obj.vel_.y < 0) {
		obj.tile_index_ = tile_map_->GetGridIndexByPos(obj.rect_.pos.x, obj.rect_.pos.y + tile_map_->tile_size_.y);
	}
}

void GameplayLayer::OnRender(float delta_time)
{
	Aegis::Renderer2D::SetProjection(camera_.view_projection_matrix_);

	Aegis::RendererClear();
	tile_map_->Render();
	player_.Render(delta_time);
	enemy_.Render(delta_time);
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
	if (!pellets_.empty()) {
		pellets_.clear();
	}

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

	SetObjectOnGrid(player_, tile_map_->player_start_pos_);
	SetObjectOnGrid(enemy_, tile_map_->enemy_start_pos_);
	enemy_.target_pos_ = GetEnemyTargetPos();

	SpawnPellets();
}

void GameplayLayer::ResetLevel()
{
	player_.moving_ = false;
	player_.vel_ = Aegis::Vec2(0, 0);
	queued_movement_ = -1;

	enemy_.moving_ = false;
	enemy_.vel_ = Aegis::Vec2(0, 0);

	SetObjectOnGrid(player_, tile_map_->player_start_pos_);
	SetObjectOnGrid(enemy_, tile_map_->enemy_start_pos_);
	enemy_.target_pos_ = GetEnemyTargetPos();

	SpawnPellets();
}

void GameplayLayer::SetObjectOnGrid(GameObject& obj, const Aegis::Vec2& pos)
{
	obj.tile_index_ = pos;
	obj.SetPosition(pos * tile_map_->tile_size_);
}

Aegis::Vec2 GameplayLayer::GetTargetTile(const GameObject& obj, Direction dir)
{
	Aegis::Vec2 target_tile;

	switch (dir)
	{
		case Direction::Up: {
			for (int y_index = obj.tile_index_.y - 1; y_index >= 0; --y_index) {
				auto* tile = tile_map_->GetTileByIndex(obj.tile_index_.x, y_index);
				if (tile->type_ == Tile::Type::Ground) {
					target_tile = tile->pos_;
					break;
				}
				else if (tile->type_ == Tile::Type::Wall) {
					target_tile = tile->pos_ + Aegis::Vec2(0, tile_map_->tile_size_.x);
					break;
				}
			}
			break;
		}
		case Direction::Down: {
			for (int y_index = obj.tile_index_.y + 1; y_index < tile_map_->grid_size_.y; ++y_index) {
				auto* tile = tile_map_->GetTileByIndex(obj.tile_index_.x, y_index);
				if (tile->type_ == Tile::Type::Ground) {
					target_tile = tile->pos_;
					break;
				}
				else if (tile->type_ == Tile::Type::Wall) {
					target_tile = tile->pos_ - Aegis::Vec2(0, tile_map_->tile_size_.y);
					break;
				}
			}
			break;
		}
		case Direction::Left: {
			for (int x_index = obj.tile_index_.x - 1; x_index >= 0; --x_index) {
				auto* tile = tile_map_->GetTileByIndex(x_index, obj.tile_index_.y);
				if (tile->type_ == Tile::Type::Ground) {
					target_tile = tile->pos_;
					break;
				}
				else if (tile->type_ == Tile::Type::Wall) {
					target_tile = tile->pos_ + Aegis::Vec2(tile_map_->tile_size_.x, 0);
					break;
				}
			}
			break;
		}
		case Direction::Right: {
			for (int x_index = obj.tile_index_.x + 1; x_index < tile_map_->grid_size_.x; ++x_index) {
				auto* tile = tile_map_->GetTileByIndex(x_index, obj.tile_index_.y);
				if (tile->type_ == Tile::Type::Ground) {
					target_tile = tile->pos_;
					break;
				}
				else if (tile->type_ == Tile::Type::Wall) {
					target_tile = tile->pos_ - Aegis::Vec2(tile_map_->tile_size_.x, 0);
					break;
				}
			}
			break;
		}
	}

	return target_tile;
}

Aegis::Vec2 GameplayLayer::GetEnemyTargetPos()
{
	Aegis::Vec2 target_pos;

	Aegis::Vec2 dir_vec = player_.rect_.pos - enemy_.rect_.pos;
	dir_vec.Normalize();

	if (abs(dir_vec.x) >= abs(dir_vec.y)) {
		dir_vec.y = 0;
	}
	else
	{
		dir_vec.x = 0;
	}
	
	Direction dir = Direction::None;
	if (dir_vec.y < 0) {
		dir = Direction::Up;
	}
	else if (dir_vec.y > 0) {
		dir = Direction::Down;
	}
	if (dir_vec.x < 0) {
		dir = Direction::Left;
	}
	else if (dir_vec.x > 0) {
		dir = Direction::Right;
	}

	enemy_.moving_ = true;

	target_pos = GetTargetTile(enemy_, dir);
	if (enemy_.rect_.pos == target_pos) {
		if (dir_vec.x == 0) {
			dir = Direction::Left;
			target_pos = GetTargetTile(enemy_, dir);
			if (enemy_.rect_.pos == target_pos) {
				dir = Direction::Right;
				target_pos = GetTargetTile(enemy_, dir);
			}
		}
		if (dir_vec.y == 0) {
			target_pos = GetTargetTile(enemy_, dir);
			dir = Direction::Down;
			if (enemy_.rect_.pos == target_pos) {
				dir = Direction::Up;
				target_pos = GetTargetTile(enemy_, dir);    
			}
		}
	}


	switch (dir)
	{
	case Direction::Up: enemy_.vel_.y = -enemy_.acceleration_;  break;
	case Direction::Down: enemy_.vel_.y = enemy_.acceleration_;  break;
	case Direction::Left: enemy_.vel_.x = -enemy_.acceleration_;  break;
	case Direction::Right: enemy_.vel_.x = enemy_.acceleration_;  break;
	}

	return target_pos;
}
