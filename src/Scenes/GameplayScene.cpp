#include "GameplayScene.h"

#include <fstream>
#include <iostream>
#include <filesystem>

GameplayScene::GameplayScene(std::shared_ptr<TileMap> tile_map)
	:ui_camera_(0, 1280, 720, 0), player_(0, 0), brutus_(0, 0), bjorn_(0, 0)
{
	camera_.SetPosition({ -144, -24});

	tile_map_ = tile_map;
	SpawnPellets();
}
GameplayScene::GameplayScene(int level)
	:ui_camera_(0, 1280, 720, 0), player_(0, 0), brutus_(0, 0), bjorn_(0, 0)
{
	if (!std::filesystem::exists("assets/levels/level" + std::to_string(level + 1) + ".txt")){
		level = 1;
	}
	LoadLevel("assets/levels/level" + std::to_string(level + 1) + ".txt");
	
	camera_.SetPosition({ -144, -24});
}

void GameplayScene::Update()
{
	if (player_.animation_.playing_) {
		player_.Update();
	}
	else if (queued_movement_ != -1) {
		HandlePlayerMovement(queued_movement_);
		queued_movement_ = -1;
	}

	if (brutus_.animation_.playing_) {
		brutus_.Update();
	}
	else {
		GetEnemyTargetPos(brutus_);
	}

	if (bjorn_.animation_.playing_) {
		bjorn_.Update();
	}
	else {
		GetEnemyTargetPos(bjorn_);
	}

	if (Aegis::AABBHasCollided(player_.sprite_.rect_, brutus_.sprite_.rect_) || Aegis::AABBHasCollided(player_.sprite_.rect_, bjorn_.sprite_.rect_)) {
		--num_lives_;
		if (num_lives_ == 0) {
			manager_->PopScene();
			return;
		}
		else {
			ResetLevel();
		}
	}

	for (auto i = pellets_.begin(); i != pellets_.end();) {
		if (Aegis::AABBHasCollided(player_.sprite_.rect_, (*i).rect_)) {
			i = pellets_.erase(i);
			if (pellets_.size() == 0) {
				player_.animation_.playing_ = false;
				queued_movement_ = -1;
				return;
			}
		}
		else {
			++i;
		}

	}
}
void GameplayScene::OnEvent(Aegis::Event& event)
{
	auto key_event = dynamic_cast<Aegis::KeyEvent*>(&event);

	if (key_event && (key_event->action_ == AE_BUTTON_PRESS || key_event->action_ == AE_BUTTON_REPEAT)) {
		auto key = key_event->key_;
		if (key == AE_KEY_ESCAPE) {
			manager_->PopScene();
			return;
		}
		if (key == AE_KEY_K) {
			ResetLevel();
			SpawnPellets();
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

void GameplayScene::Render(float delta_time)
{
	Aegis::Renderer2D::SetProjection(camera_.view_projection_matrix_);
	Aegis::RendererClear();
	tile_map_->Render();
	player_.Render(delta_time);
	brutus_.Render(delta_time);
	bjorn_.Render(delta_time);
	for (auto& pellet : pellets_)
	{
		pellet.Render(0.0f);
	}
	Aegis::Renderer2D::EndScene();

	Aegis::Renderer2D::BeginScene(ui_camera_.view_projection_matrix_);

	for (int i = 0; i < num_lives_; ++i) {
		Aegis::DrawQuad({ 20 + (float) i * 22, 675 }, { 20, 20 }, { 1.0f, 0.1f, 0.1f, 1.0f });
	}

}

void GameplayScene::HandlePlayerMovement(int key_code)
{
	Direction dir = Direction::None;
	switch (key_code)
	{
	case GLFW_KEY_UP: dir = Direction::Up;  break;
	case GLFW_KEY_DOWN: dir = Direction::Down;  break;
	case GLFW_KEY_LEFT: dir = Direction::Left;  break;
	case GLFW_KEY_RIGHT: dir = Direction::Right;  break;
	}

	if (player_.grid_coord_ == GetSlidingTargetTile(player_.grid_coord_, dir)) {
		return;
	}
	else {
		player_.grid_coord_ = GetSlidingTargetTile(player_.grid_coord_, dir);
		player_.StartMoving();
	}
}

void GameplayScene::GetEnemyTargetPos(GameObject& obj)
{
	obj.grid_coord_ = GetTargetTileCoordBFS(obj.grid_coord_, player_.grid_coord_, obj.slides_on_ice_);
	obj.StartMoving();
}

Aegis::Vec2 GameplayScene::GetSlidingTargetTile(const Aegis::Vec2& start, Direction dir) const
{
	int x_index = start.x;
	int y_index = start.y;

	switch (dir)
	{
	case Direction::Up: {
		//Move past ice, move back against wall, stand on ground
		--y_index;
		while (tile_map_->GetTileByIndex(start.x, y_index)->type_ == Tile::Type::Ice) {
			--y_index;
		}
		if (tile_map_->GetTileByIndex(start.x, y_index)->type_ == Tile::Type::Wall) {
			++y_index;
		}
		break;
	}
	case Direction::Down: {
		++y_index;
		while (tile_map_->GetTileByIndex(start.x, y_index)->type_ == Tile::Type::Ice) {
			++y_index;
		}
		if (tile_map_->GetTileByIndex(start.x, y_index)->type_ == Tile::Type::Wall) {
			--y_index;
		}
		break;
	}
	case Direction::Left: {
		--x_index;
		while (tile_map_->GetTileByIndex(x_index, start.y)->type_ == Tile::Type::Ice) {
			--x_index;
		}
		if (tile_map_->GetTileByIndex(x_index, start.y)->type_ == Tile::Type::Wall) {
			++x_index;
		}
		break;
	}
	case Direction::Right: {
		++x_index;
		while (tile_map_->GetTileByIndex(x_index, start.y)->type_ == Tile::Type::Ice) {
			++x_index;
		}
		if (tile_map_->GetTileByIndex(x_index, start.y)->type_ == Tile::Type::Wall) {
			--x_index;
		}
		break;
	}
	}

	return Aegis::Vec2(x_index, y_index);
}

std::string ToString(const Aegis::Vec2 val) {
	return std::to_string(val.x) + std::to_string(val.y);
}

Aegis::Vec2 GameplayScene::GetTargetTileCoordBFS(const Aegis::Vec2& start, const Aegis::Vec2& end, bool slides) const
{
	std::vector<Aegis::Vec2> frontier;
	frontier.push_back(start);
	std::unordered_map<std::string, Aegis::Vec2> parent;
	parent[ToString(start)] = start;
	while (!frontier.empty()) {
		auto current = frontier[0];
		frontier.erase(frontier.begin());

		if (current == end) {
			break;
		}
		std::vector<Aegis::Vec2> neighbors;
		if (slides) {
			neighbors = GetNeighborTilesSliding(current);
		}
		else {
			neighbors = GetNeighborTilesMoving(current);
		}

		for (auto& neighbor : neighbors) {
			if (parent.find(ToString(neighbor)) == parent.end()) {
				frontier.push_back(neighbor);
				parent[ToString(neighbor)] = current;
			}
		}
	}
	//if no path available
	if (parent[ToString(end)] == Aegis::Vec2(0, 0)) {
		std::cout << "Unable to find path BFS";
		return start;
	}

	Aegis::Vec2 path_start = end;
	while (parent[ToString(path_start)] != start) {
		path_start = parent[ToString(path_start)];
	}

	return path_start;
}

void GameplayScene::SetObjectOnGrid(GameObject& obj, const Aegis::Vec2& pos)
{
	obj.grid_coord_ = pos;
	obj.SetPosition(pos * tile_map_->tile_size_);
}

std::vector<Aegis::Vec2> GameplayScene::GetNeighborTilesSliding(const Aegis::Vec2& tile) const
{
	std::vector<Aegis::Vec2> neighbors;
	neighbors.push_back(GetSlidingTargetTile(tile, Direction::Up));
	neighbors.push_back(GetSlidingTargetTile(tile, Direction::Down));
	neighbors.push_back(GetSlidingTargetTile(tile, Direction::Left));
	neighbors.push_back(GetSlidingTargetTile(tile, Direction::Right));

	return neighbors;
}

std::vector<Aegis::Vec2> GameplayScene::GetNeighborTilesMoving(const Aegis::Vec2& tile) const
{
	auto adjacent_tiles = tile_map_->GetAdjacentTiles(tile);

	std::vector<Aegis::Vec2> neighbors;
	for (auto adj : adjacent_tiles){
		if (adj->type_ != Tile::Wall){
			neighbors.push_back(adj->pos_ / 32);
		}
	}

	return neighbors;
}

void GameplayScene::SpawnPellets()
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

void GameplayScene::LoadLevel(const std::string& file_path)
{
	tile_map_ = std::make_unique<TileMap>(file_path, 32);

	SetObjectOnGrid(player_, tile_map_->bruce_spawn_index_);
	SetObjectOnGrid(brutus_, tile_map_->brutus_spawn_index_);
	SetObjectOnGrid(bjorn_, tile_map_->bjorn_spawn_index_);
	GetEnemyTargetPos(brutus_);
	GetEnemyTargetPos(bjorn_);

	SpawnPellets();
}

void GameplayScene::ResetLevel()
{
	queued_movement_ = -1;

	player_.animation_.Stop();
	brutus_.animation_.Stop();
	bjorn_.animation_.Stop();

	SetObjectOnGrid(player_, tile_map_->bruce_spawn_index_);
	SetObjectOnGrid(brutus_, tile_map_->brutus_spawn_index_);
	SetObjectOnGrid(bjorn_, tile_map_->bjorn_spawn_index_);
}

