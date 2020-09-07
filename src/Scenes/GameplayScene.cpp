#include "GameplayScene.h"

#include <fstream>
#include <iostream>
#include <filesystem>

GameplayScene::GameplayScene(std::shared_ptr<TileMap> tile_map)
	:ui_camera_(0, 1280, 720, 0), player_(0, 0), brutus_(0, 0), bjorn_(0, 0)
{
	tile_map_ = tile_map;
	SetUpLevel();

	camera_.SetPosition({ -144, -24});
}
GameplayScene::GameplayScene(int level)
	:ui_camera_(0, 1280, 720, 0), player_(0, 0), brutus_(0, 0), bjorn_(0, 0)
{
	auto atlas = Aegis::TextureManager::Load("assets/textures/tundra-tile-map.png");
	tile_map_ = std::make_shared<TileMap>("assets/levels/level" + std::to_string(level) + ".txt", 32, atlas);
	SetUpLevel();
	
	camera_.SetPosition({ -144, -24});
}

void GameplayScene::Update()
{
	//player_.Update();
	//brutus_.Update();
	//bjorn_.Update();
	//
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
			HandlePlayerMovement(key);
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

	if (player_.animation_.playing_){
		player_.MoveTo(GetSlidingTargetTile(player_.target_grid_index_, dir));
	}
	else{
		player_.MoveTo(GetSlidingTargetTile(player_.grid_index_, dir));
	}
}

void GameplayScene::GetEnemyTargetPos(GameObject& obj)
{
	obj.MoveTo(GetTargetTileCoordBFS(obj.grid_index_, player_.grid_index_, obj.slides_on_ice_));
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
		const Tile* tile = tile_map_->GetTileByIndex(start.x, y_index);
		while (tile && tile->is_slippery_) {
			--y_index;
			tile = tile_map_->GetTileByIndex(start.x, y_index);
		}
		if (!tile || tile->is_solid_) {
			++y_index;
		}
		break;
	}
	case Direction::Down: {
		++y_index;

		const Tile* tile = tile_map_->GetTileByIndex(start.x, y_index);
		while (tile && tile->is_slippery_) {
			++y_index;
			tile = tile_map_->GetTileByIndex(start.x, y_index);
		}
		if (!tile || tile->is_solid_) {
			--y_index;
		}
		break;
	}
	case Direction::Left: {
		--x_index;
		const Tile* tile = tile_map_->GetTileByIndex(x_index, start.y);
		while (tile && tile->is_slippery_) {
			--x_index;
			tile = tile_map_->GetTileByIndex(x_index, start.y);
		}
		if (!tile || tile->is_solid_) {
			++x_index;
		}
		break;
	}
	case Direction::Right: {
		++x_index;
		const Tile* tile = tile_map_->GetTileByIndex(x_index, start.y);
		while (tile && tile->is_slippery_) {
			++x_index;
			tile = tile_map_->GetTileByIndex(x_index, start.y);
		}
		if (!tile || tile->is_solid_) {
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
	//TODO: Use a 2D array of Vec2 for parent
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
	std::cout << "parent map size: " << parent.size() << '\n';
	return path_start;
}

void GameplayScene::SetObjectOnGrid(GameObject& obj, const Aegis::Vec2& pos)
{
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
	auto adjacent_indices = tile_map_->GetAdjacentTilesIndices(tile);

	std::vector<Aegis::Vec2> neighbors;
	for (auto index : adjacent_indices){
		if (!tile_map_->GetTileByIndex(index.x, index.y)->is_solid_){
			neighbors.push_back(index);
		}
	}

	return neighbors;
}

void GameplayScene::SpawnPellets()
{
	if (!pellets_.empty()) {
		pellets_.clear();
	}

	for (int i = 0; i < tile_map_->grid_size_.x; ++i){
		for (int j = 0; j < tile_map_->grid_size_.y; ++j){
			if (tile_map_->GetTileByIndex(i, j)->is_slippery_){
				pellets_.emplace_back(Pellet((i * 32) + 12, (j * 32) + 12));
			}
		}
	}
}

void GameplayScene::SetUpLevel()
{
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

