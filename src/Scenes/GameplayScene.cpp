#include "GameplayScene.h"
#include "OptionsScene.h"
#include "LevelSelectScene.h"

#include <fstream>
#include <iostream>
#include <filesystem>

GameplayScene::GameplayScene(std::shared_ptr<TileMap> tile_map, GameData& game_data)
	:player_(0, 0), brutus_(0, 0), bjorn_(0, 0), tile_map_(tile_map), game_data_(game_data)
{
	Init();
}
GameplayScene::GameplayScene(int level, GameData& game_data)
	:player_(0, 0), brutus_(0, 0), bjorn_(0, 0), level_(level), game_data_(game_data)
{
	auto atlas = Aegis::TextureManager::Load("assets/textures/tile_map.png");
	std::string level_file = "assets/levels/level_" + std::to_string(level) + ".txt";
	tile_map_ = std::make_shared<TileMap>(level_file, 32, atlas);

	Init();
}

GameplayScene::~GameplayScene()
{
	Aegis::AudioPlayer::StopSound(bg_music_);
}

void GameplayScene::Init()
{
	bg_music_ = Aegis::AudioPlayer::LoadSound("assets/audio/gameplay_bgm.ogg", true, true);
	fish_sfx_ = Aegis::AudioPlayer::LoadSound("assets/audio/fish_collect.ogg");
	death_sfx_ = Aegis::AudioPlayer::LoadSound("assets/audio/death.ogg");
	game_over_sfx_ = Aegis::AudioPlayer::LoadSound("assets/audio/lose.ogg");
	level_complete_sfx_ = Aegis::AudioPlayer::LoadSound("assets/audio/level_complete.ogg");

	Aegis::AudioPlayer::PlaySound(bg_music_, 90);
	camera_.SetPosition({ -144, -24});

	bg_texture_ = Aegis::TextureManager::Load("assets/textures/frame_bg.png");
	auto sprite_sheet = Aegis::TextureManager::Load("assets/textures/tile_map.png");

	ui_layer_ = std::make_unique<Aegis::UILayer>();

	//Scoreboard
	ui_layer_->AddWidget<Aegis::SpriteWidget>(Aegis::Vec2(20, 16), Aegis::TextureManager::Load("assets/textures/score_frame.png"));
	auto scoreboard_container = ui_layer_->AddWidget<Aegis::Container>(Aegis::AABB(20, 16, 98, 128), Aegis::Container::Vertical, 4, Aegis::Alignment::VCenter | Aegis::Alignment::HCenter);
	scoreboard_container->AddWidget<Aegis::Label>("Lives:", Aegis::Vec2(), Aegis::Vec4(0,0,0,1));
	auto heart_container = scoreboard_container->AddWidget<Aegis::Container>(Aegis::AABB( 0, 0, 98, 26 ), Aegis::Container::Horizontal, 4, Aegis::Alignment::VCenter | Aegis::Alignment::HCenter);
	for (int i = 0; i < max_lives_; ++i) {
		heart_widgets_[i] = heart_container->AddWidget<Aegis::SpriteWidget>(Aegis::Vec2(), sprite_sheet, Aegis::AABB(128, 112, 16, 16));
	}
	total_pellets_ = tile_map_->pellet_spawn_indices_.size();
	scoreboard_container->AddWidget<Aegis::SpriteWidget>(Aegis::Vec2(), sprite_sheet, Aegis::AABB(96, 96, 32, 32));
	pellet_count_label_ = scoreboard_container->AddWidget<Aegis::Label>(std::to_string(pellets_collected_) + "/" + std::to_string(total_pellets_), Aegis::Vec2(), Aegis::Vec4(0, 0, 0, 1));

	countdown_.ConnectSignal("done", [&]() {countdown_label_->visible_ = false; stopwatch_.Start(); });
	countdown_.Start(2500);
	countdown_label_ = ui_layer_->AddWidget<Aegis::Label>(std::to_string((int)countdown_.GetTimeInSeconds() + 1), Aegis::Vec2(600, 300), Aegis::Vec4(0.0f,0.0f, 0.0f, 1.0f));
	auto countdown_font = Aegis::FontManager::Load("assets/fonts/roboto_regular.ttf", 128);
	countdown_label_->SetFont(countdown_font);

	pause_menu_ = ui_layer_->AddWidget<PauseMenu>(Aegis::AABB{ 400, 400, 240, 350 });
	pause_menu_->continue_button_->ConnectSignal("pressed", [&]() {Resume(); });
	pause_menu_->retry_button_->ConnectSignal("pressed", [&]() {SetUpLevel(); Resume(); });
	pause_menu_->options_button_->ConnectSignal("pressed", [&]() {manager_->PushScene<OptionsScene>(); });
	pause_menu_->quit_button_->ConnectSignal("pressed", [&]() {manager_->PopScene(); });

	game_over_dialog_ = ui_layer_->AddWidget<Aegis::Dialog>("You lose. Try Again?", Aegis::AABB(400, 200, 300, 300));
	game_over_dialog_->AddButton("Retry", [&]() {SetUpLevel(); });
	game_over_dialog_->AddButton("Main Menu", [&]() {manager_->PopScene(); });

	level_complete_dialog_ = ui_layer_->AddWidget<ScoreCard>("Congratulations, you won!", Aegis::AABB(400, 200, 300, 300));
	level_complete_dialog_->AddButton("Next Level", [&]() {manager_->ReplaceScene<GameplayScene>(level_ + 1, game_data_); });
	level_complete_dialog_->AddButton("Replay Level", [&]() {SetUpLevel(); });
	level_complete_dialog_->AddButton("Main Menu", [&]() {manager_->PopScene(); });

	game_complete_dialog_ = ui_layer_->AddWidget<ScoreCard>("Congratulations! You beat the game!", Aegis::AABB(400, 200, 300, 300));
	game_complete_dialog_->AddButton("Level Select", [&]() {manager_->ReplaceScene<LevelSelectScene>(game_data_); });
	game_complete_dialog_->AddButton("Main Menu", [&]() {manager_->PopScene(); });

	SetUpLevel();
	for (auto& pos : tile_map_->pellet_spawn_indices_){
		pellets_.emplace_back(Pellet((pos.x * 32) + 12, (pos.y * 32) + 12));
	}
}

void GameplayScene::Update()
{
	if (paused_) return;

	if (!countdown_.IsStopped()){
		countdown_.Update();
		//countdown + 1 so that it goes from 3, 2, 1 instead of 2, 1, 0
		countdown_label_->SetText(std::to_string((int)countdown_.GetTimeInSeconds() + 1));
		return;
	}

	stopwatch_.Update();
	player_.Update();

	brutus_.Update();
	if (!brutus_.IsMoving()){
		brutus_.MoveTo(GetEnemyTargetPos(brutus_));
	}

	bjorn_.Update();
	if (!bjorn_.IsMoving()) {
		bjorn_.MoveTo(GetEnemyTargetPos(bjorn_));
	}


	if (Aegis::AABBHasCollided(player_.GetRect(), brutus_.GetRect()) 
		|| Aegis::AABBHasCollided(player_.GetRect(), bjorn_.GetRect())) {
			RemoveLife();
	}

	for (auto& pellet : pellets_){
		if (pellet.visible_ && Aegis::AABBHasCollided(player_.GetRect(), pellet.GetRect())) {
			pellet.visible_ = false;
			Aegis::AudioPlayer::PlaySound(fish_sfx_, 80);
			++pellets_collected_; 
			UpdatePelletCount();

			if (pellets_collected_ == total_pellets_){
				paused_ = true;
				Aegis::AudioPlayer::PlaySound(level_complete_sfx_);
				double completion_time = stopwatch_.GetTimeInSeconds();
				if (level_ == game_data_.num_levels_) {
					game_complete_dialog_->Show(completion_time, GetNumStarsEarned(completion_time));
				}
				else {
					level_complete_dialog_->Show(completion_time, GetNumStarsEarned(completion_time));
				}
			}
		}
	}
}

int GameplayScene::GetNumStarsEarned(double time)
{
	if (time <= game_data_.star_thresholds_[level_][0]){
		return 3;
	} else if (time <= game_data_.star_thresholds_[level_][1]){
		return 2;
	} else {
		return 1;
	}
}

void GameplayScene::OnEvent(Aegis::Event& event)
{
	auto key_event = dynamic_cast<Aegis::KeyEvent*>(&event);

	if (key_event && (key_event->action_ == AE_BUTTON_PRESS || key_event->action_ == AE_BUTTON_REPEAT)) {
		auto key = key_event->key_;
		if (key == AE_KEY_ESCAPE) {
			Pause();
		}
		if (key == AE_KEY_K) {
			SetUpLevel();
		}
		if (key == AE_KEY_UP || key == AE_KEY_DOWN || key == AE_KEY_LEFT || key == AE_KEY_RIGHT) {
			HandlePlayerMovement(key);
		}
	}
}

void GameplayScene::Pause()
{
	countdown_.Stop();
	stopwatch_.Stop();
	paused_ = true;
	pause_menu_->visible_ = true;
}

void GameplayScene::Resume()
{
	countdown_.Resume();
	stopwatch_.Start();
	paused_ = false;
	pause_menu_->visible_ = false;
}

void GameplayScene::Render(float delta_time)
{
	Aegis::Renderer2D::SetProjection(camera_.view_projection_matrix_);

	Aegis::DrawQuad({ -144, -24 }, *bg_texture_);
	tile_map_->Render();

	for (auto& pellet : pellets_) {
		if (pellet.visible_){
			pellet.Render(delta_time);
		}
	}

	player_.Render(delta_time);
	brutus_.Render(delta_time);
	bjorn_.Render(delta_time);
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
		player_.MoveTo(GetSlidingTargetTile(player_.GetGridIndex(), dir));
	}
}

Aegis::Vec2 GameplayScene::GetEnemyTargetPos(GameObject& obj)
{
	if (obj.slides_on_ice_) {
		return GetTargetTileCoordBFS(obj.GetGridIndex(), player_.target_grid_index_, obj.slides_on_ice_);
	}
	else {
		return GetTargetTileCoordBFS(obj.GetGridIndex(), player_.GetGridIndex(), obj.slides_on_ice_);
	}
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
		std::vector<Aegis::Vec2> neighbors = slides ? GetNeighborTilesSliding(current) : GetNeighborTilesMoving(current);

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

void GameplayScene::ResetObjectPositions()
{
	SetObjectOnGrid(player_, tile_map_->bruce_spawn_index_);
	SetObjectOnGrid(brutus_, tile_map_->brutus_spawn_index_);
	SetObjectOnGrid(bjorn_, tile_map_->bjorn_spawn_index_);
	player_.target_grid_index_ = player_.GetGridIndex();
	player_.queued_movement_ = {-1, -1};
	GetEnemyTargetPos(brutus_);
	GetEnemyTargetPos(bjorn_);
	player_.animation_.Stop();
	brutus_.animation_.Stop();
	bjorn_.animation_.Stop();

	countdown_label_->visible_ = true;
	countdown_.Start(3000);
	stopwatch_.Stop();
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
	for (auto& pellet : pellets_){
		pellet.visible_ = true;
	}
}

void GameplayScene::SetUpLevel()
{
	stopwatch_.Restart();
	ResetObjectPositions();
	SpawnPellets();

	num_lives_ = 3;
	for (auto& heart : heart_widgets_) {
		heart->sprite_.SetSubTextureRect({ 128, 112, 16, 16 });
	}

	pellets_collected_ = 0;
	UpdatePelletCount();
	paused_ = false;
}

void GameplayScene::RemoveLife()
{
	--num_lives_;

	heart_widgets_[num_lives_]->sprite_.SetSubTextureRect({ 128, 96, 16, 16 });
	if (num_lives_ == 0) {
		game_over_dialog_->visible_ = true;
		paused_ = true;
		Aegis::AudioPlayer::PlaySound(game_over_sfx_);
		return;
	}
	else {
		Aegis::AudioPlayer::PlaySound(death_sfx_);
		ResetObjectPositions();
	}
}

void GameplayScene::UpdatePelletCount()
{
	pellet_count_label_->SetText(std::to_string(pellets_collected_) + "/" + std::to_string(total_pellets_));
}
