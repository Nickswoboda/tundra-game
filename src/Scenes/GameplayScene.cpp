#include "GameplayScene.h"

#include <fstream>
#include <iostream>
#include <filesystem>

GameplayScene::GameplayScene(std::shared_ptr<TileMap> tile_map)
	:player_(0, 0), brutus_(0, 0), bjorn_(0, 0)
{
	tile_map_ = tile_map;

	camera_.SetPosition({ -144, -24});

	ui_layer_ = std::make_unique<Aegis::UILayer>();
	for (int i = 0; i < max_lives_; ++i){
		heart_textures_[i] = ui_layer_->AddWidget<Aegis::TextureWidget>(new Aegis::TextureWidget({1.0f, 0.0f, 0.0f, 1.0f}, { 50.0f + (i * 30), 50}, {25.0f, 25.0f}));
	}
	SetUpLevel();
}
GameplayScene::GameplayScene(int level)
	:player_(0, 0), brutus_(0, 0), bjorn_(0, 0)
{
	auto atlas = Aegis::TextureManager::Load("assets/textures/tundra-tile-map.png");
	tile_map_ = std::make_shared<TileMap>("assets/levels/level" + std::to_string(level) + ".txt", 32, atlas);
	
	camera_.SetPosition({ -144, -24});

	ui_layer_ = std::make_unique<Aegis::UILayer>();
	for (int i = 0; i < max_lives_; ++i){
		heart_textures_[i] = ui_layer_->AddWidget<Aegis::TextureWidget>(new Aegis::TextureWidget({1.0f, 0.0f, 0.0f, 1.0f}, { 50.0f + (i * 30), 50}, {25.0f, 25.0f}));
	}
	SetUpLevel();
}

void GameplayScene::Update()
{
	player_.Update();

	brutus_.Update();
	if (!brutus_.IsMoving()){
		brutus_.MoveTo(GetEnemyTargetPos(brutus_));
	}

	bjorn_.Update();
	if (!bjorn_.IsMoving()) {
		bjorn_.MoveTo(GetEnemyTargetPos(bjorn_));
	}

	if (Aegis::AABBHasCollided(player_.sprite_.rect_, brutus_.sprite_.rect_) || Aegis::AABBHasCollided(player_.sprite_.rect_, bjorn_.sprite_.rect_)) {
		RemoveLife();
		if (num_lives_ == 0){
			manager_->PopScene();
			return;
		}
		else{
			ResetLevel();
		}
	}

	for (auto i = pellets_.begin(); i != pellets_.end();) {
		if (Aegis::AABBHasCollided(player_.sprite_.rect_, (*i).rect_)) {
			i = pellets_.erase(i);
			if (pellets_.size() == 0) {
				player_.animation_.playing_ = false;
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
}

void GameplayScene::HandlePlayerMovement(int key_code)
{
	Aegis::Vec2 dir;
	switch (key_code)
	{
	case GLFW_KEY_UP: dir = {0, -1};  break;
	case GLFW_KEY_DOWN: dir = {0, 1}; break;
	case GLFW_KEY_LEFT: dir = {-1, 0};  break;
	case GLFW_KEY_RIGHT: dir = {1, 0};  break;
	}

	//if already moving, uses target_index to queue up movement
	if (player_.IsMoving()){
		player_.MoveTo(GetSlidingTargetTile(player_.target_grid_index_, dir));
	}
	else{
		player_.MoveTo(GetSlidingTargetTile(player_.grid_index_, dir));
	}
}

Aegis::Vec2 GameplayScene::GetEnemyTargetPos(GameObject& obj)
{
	return GetTargetTileCoordBFS(obj.grid_index_, player_.grid_index_, obj.slides_on_ice_);
}

Aegis::Vec2 GameplayScene::GetSlidingTargetTile(const Aegis::Vec2& start, const Aegis::Vec2& dir) const
{
	Aegis::Vec2 pos = start;

	//Check each tile, if ice, go to next tile. If ground, stop, if wall or edge of map, go back one tile
	pos += dir;
	const Tile* tile = tile_map_->GetTileByIndex(pos.x, pos.y); 
	while (tile && tile->is_slippery_){
		pos += dir;
		tile = tile_map_->GetTileByIndex(pos.x, pos.y); 
	}
	if (!tile || tile->is_solid_){
		pos -= dir;
	}

	return Aegis::Vec2(pos.x, pos.y);
}


Aegis::Vec2 GameplayScene::GetTargetTileCoordBFS(const Aegis::Vec2& start, const Aegis::Vec2& end, bool slides) const
{
	std::vector<Aegis::Vec2> frontier;
	frontier.push_back(start);

	std::vector<std::vector<Aegis::Vec2>> parent(tile_map_->grid_size_.x, std::vector<Aegis::Vec2>(tile_map_->grid_size_.y, Aegis::Vec2(-1, -1)));
	parent[start.x][start.y] = start;

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
			//if not already in list
			if (parent[neighbor.x][neighbor.y].x == -1 ) {
				frontier.push_back(neighbor);
				parent[neighbor.x][neighbor.y] = current;
			}
		}
	}
	//if no path available
	if (parent[end.x][end.y].x == -1) {
		std::cout << "Unable to find path BFS";
		return start;
	}

	Aegis::Vec2 path_start = end;
	while (parent[path_start.x][path_start.y] != start) {
		path_start = parent[path_start.x][path_start.y];
	}
	return path_start;
}

void GameplayScene::SetObjectOnGrid(GameObject& obj, const Aegis::Vec2& pos)
{
	obj.SetPosition(pos * tile_map_->tile_size_);
}

std::vector<Aegis::Vec2> GameplayScene::GetNeighborTilesSliding(const Aegis::Vec2& tile) const
{
	std::vector<Aegis::Vec2> neighbors;
	neighbors.push_back(GetSlidingTargetTile(tile, {0, -1}));
	neighbors.push_back(GetSlidingTargetTile(tile, {0, 1}));
	neighbors.push_back(GetSlidingTargetTile(tile, {-1, 0}));
	neighbors.push_back(GetSlidingTargetTile(tile, {1, 0}));

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
	player_.animation_.Stop();
	brutus_.animation_.Stop();
	bjorn_.animation_.Stop();

	SetUpLevel();
}

void GameplayScene::RemoveLife()
{
	--num_lives_;
	heart_textures_[num_lives_]->visible_ = false;
}
