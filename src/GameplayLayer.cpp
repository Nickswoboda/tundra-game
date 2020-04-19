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
	camera_.SetPosition({ -144, -24, 0 });
	Aegis::Renderer2D::SetProjection(camera_.view_projection_matrix_);
}

void GameplayLayer::OnUpdate()
{
	if (player_.animation_.playing_) {
		player_.Update();
	}
	else if (queued_movement_ != -1) {
		HandlePlayerMovement(queued_movement_);
		queued_movement_ = -1;
	}

	if (enemy_.animation_.playing_) {
		enemy_.Update();
	}
	else {
		GetEnemyTargetPos();
	}

	if (Aegis::AABBHasCollided(player_.rect_, enemy_.rect_)) {
		ResetLevel();
	}

	for (auto i = pellets_.begin(); i != pellets_.end();) {
		if (Aegis::AABBHasCollided(player_.rect_, (*i).rect_)) {
			i = pellets_.erase(i);
			if (pellets_.size() == 0) {
				player_.animation_.playing_ = false;
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
		auto key = key_event->key_;
		if (key == AE_KEY_J) {
			SaveLevel();
		}
		if (key == AE_KEY_K) {
			ResetLevel();
			SpawnPellets();
		}

		if (key == AE_KEY_R || key == AE_KEY_T || key == AE_KEY_Y) {
			auto mouse_pos = Aegis::Application::GetMousePos();
			auto tile = tile_map_->GetTileByPos(mouse_pos.x, mouse_pos.y);
		
			if (tile != nullptr) {
				auto index = tile_map_->GetTileIndex(*tile);
				
				Aegis::Vec2 tile_spawn_pos = index * tile_map_->tile_size_;
				switch (key)
				{
				case AE_KEY_R: *tile = Wall(tile_spawn_pos.x, tile_spawn_pos.y); break;
				case AE_KEY_T: *tile = Ice(tile_spawn_pos.x, tile_spawn_pos.y); break;
				case AE_KEY_Y: *tile = Ground(tile_spawn_pos.x, tile_spawn_pos.y); break;
				}
			}
		}
		if (key == AE_KEY_UP || key == AE_KEY_DOWN || key == AE_KEY_LEFT || key == AE_KEY_RIGHT) {
			if (!player_.animation_.playing_) {
				HandlePlayerMovement(key);
			}
			else {
				queued_movement_ = key;
			}
		}
	}
}

void GameplayLayer::OnRender(float delta_time)
{
	Aegis::RendererClear();

	tile_map_->Render();
	player_.Render(delta_time);
	enemy_.Render(delta_time);
	for (auto& pellet : pellets_)
	{
		pellet.Render(0.0f);
	}
	Aegis::DrawText(std::to_string(Aegis::Application::GetFrameTime()), { 0, 0 }, { 1.0f, 1.0f, 1.0f, 1.0f });
}

void GameplayLayer::HandlePlayerMovement(int key_code)
{
	Direction dir = Direction::None;
	switch (key_code)
	{
	case GLFW_KEY_UP: dir = Direction::Up;  break;
	case GLFW_KEY_DOWN: dir = Direction::Down;  break;
	case GLFW_KEY_LEFT: dir = Direction::Left;  break;
	case GLFW_KEY_RIGHT: dir = Direction::Right;  break;
	}

	if (player_.grid_coord_ == GetTargetTileCoord(player_, dir)) {
		return;
	}
	else {
		player_.grid_coord_ = GetTargetTileCoord(player_, dir);
		player_.StartMoving();
		//player_.move_animation.Play();
		//lerp rect.pos to pos of new grid coords
	}
}

void GameplayLayer::GetEnemyTargetPos()
{
	Aegis::Vec2 dir_vec = player_.rect_.pos - enemy_.rect_.pos;

	//go in the direction with largest magnitude
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

	while (enemy_.grid_coord_ == GetTargetTileCoord(enemy_, dir)) {
		//if already moving up or down, try left and right
		if (dir_vec.x == 0) {
			dir = Direction::Left;
			if (enemy_.grid_coord_ == GetTargetTileCoord(enemy_, dir)) {
				dir = Direction::Right;
			}
		}
		//if already moving left or right, try up and down
		if (dir_vec.y == 0) {
			dir = Direction::Up;
			if (enemy_.grid_coord_ == GetTargetTileCoord(enemy_, dir)) {
				dir = Direction::Down;
			}
		}
	}

	enemy_.grid_coord_ = GetTargetTileCoord(enemy_, dir);;
	enemy_.StartMoving();
	//player_.move_animation.Play();
	//lerp rect.pos to pos of new grid coords
}

Aegis::Vec2 GameplayLayer::GetTargetTileCoord(const GameObject& obj, Direction dir)
{
	int x_index = obj.grid_coord_.x;
	int y_index = obj.grid_coord_.y;

	switch (dir)
	{
		case Direction::Up: {
			//Move past ice, move back against wall, stand on ground
			--y_index;
			while (tile_map_->GetTileByIndex(obj.grid_coord_.x, y_index)->type_ == Tile::Type::Ice) {
				--y_index;
			}
			if (tile_map_->GetTileByIndex(obj.grid_coord_.x, y_index)->type_ == Tile::Type::Wall) {
				++y_index;
			}
			break;
		}
		case Direction::Down: {
			++y_index;
			while (tile_map_->GetTileByIndex(obj.grid_coord_.x, y_index)->type_ == Tile::Type::Ice) {
				++y_index;
			}
			if (tile_map_->GetTileByIndex(obj.grid_coord_.x, y_index)->type_ == Tile::Type::Wall) {
				--y_index;
			}
			break;
		}
		case Direction::Left: {
			--x_index;
			while (tile_map_->GetTileByIndex(x_index, obj.grid_coord_.y)->type_ == Tile::Type::Ice) {
				--x_index;
			}
			if (tile_map_->GetTileByIndex(x_index, obj.grid_coord_.y)->type_ == Tile::Type::Wall) {
				++x_index;
			}
			break;
		}
		case Direction::Right: {
			++x_index;
			while (tile_map_->GetTileByIndex(x_index, obj.grid_coord_.y)->type_ == Tile::Type::Ice) {
				++x_index;
			}
			if (tile_map_->GetTileByIndex(x_index, obj.grid_coord_.y)->type_ == Tile::Type::Wall) {
				--x_index;
			}
			break;
		}
	}

	return Aegis::Vec2(x_index, y_index);
}

void GameplayLayer::SetObjectOnGrid(GameObject& obj, const Aegis::Vec2& pos)
{
	obj.grid_coord_ = pos;
	obj.SetPosition(pos * tile_map_->tile_size_);
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
	GetEnemyTargetPos();

	SpawnPellets();
}

void GameplayLayer::ResetLevel()
{
	player_.animation_.playing_ = false;
	queued_movement_ = -1;
	player_.animation_.timer_.Stop();

	enemy_.animation_.playing_ = false;
	enemy_.animation_.timer_.Stop();

	SetObjectOnGrid(player_, tile_map_->player_start_pos_);
	SetObjectOnGrid(enemy_, tile_map_->enemy_start_pos_);
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

