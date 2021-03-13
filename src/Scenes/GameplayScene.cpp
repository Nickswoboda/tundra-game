#include "GameplayScene.h"
#include "OptionsScene.h"
#include "LevelSelectScene.h"
#include "../PathFinding.h"
#include "../Utilities.h"

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

	ui_layer_ = std::make_unique<Aegis::UILayer>();

	total_pellets_ = tile_map_->pellet_spawn_indices_.size();
	score_board_ = ui_layer_->AddWidget<ScoreBoard>(num_lives_, total_pellets_);

	countdown_.ConnectSignal("done", [&]() {countdown_label_->visible_ = false; stopwatch_.Start(); });
	countdown_.Start(2500);
	countdown_label_ = ui_layer_->AddWidget<Aegis::Label>(std::to_string((int)countdown_.GetTimeInSeconds() + 1), Aegis::Vec2(600, 300), Aegis::Vec4(0.0f,0.0f, 0.0f, 1.0f));
	auto countdown_font = Aegis::FontManager::Load("assets/fonts/roboto_regular.ttf", 128);
	countdown_label_->SetFont(countdown_font);

	pause_menu_ = ui_layer_->AddWidget<PauseMenu>(*this);
	game_over_dialog_ = ui_layer_->AddWidget<GameOverDialog>(*this);
	score_dialog_ = ui_layer_->AddWidget<ScoreDialog>(*this);

	info_dialog_ = ui_layer_->AddWidget<InfoDialog>();
	info_dialog_->ConnectSignal("closed", [&]() { 
		if (!pause_menu_->visible_) {
			game_data_.first_time_playing_ = false;  Resume(); 
		}
	});

	for (auto& pos : tile_map_->pellet_spawn_indices_){
		pellets_.emplace_back((pos.x * 32) + 12, (pos.y * 32) + 12);
	}

	SetUpLevel();
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
	score_board_->SetTimeLabel(FormatTime(stopwatch_.GetTimeInSeconds()));
	player_.Update();

	brutus_.Update();
	if (!brutus_.IsMoving()){
		auto target_pos = GetTargetTileCoordBFS(*tile_map_, brutus_.GetGridIndex(), player_.GetGridIndex(), brutus_.slides_on_ice_);
		brutus_.MoveTo(target_pos);
	}

	bjorn_.Update();
	if (!bjorn_.IsMoving()) {
		auto target_pos = GetTargetTileCoordBFS(*tile_map_, bjorn_.GetGridIndex(), player_.target_grid_index_, bjorn_.slides_on_ice_);
		bjorn_.MoveTo(target_pos);
	}

	if (Aegis::AABBHasCollided(player_.GetRect(), brutus_.GetRect()) 
		|| Aegis::AABBHasCollided(player_.GetRect(), bjorn_.GetRect())) {
			RemoveLife();
	}

	for (auto& pellet : pellets_){
		if (pellet.visible_ && Aegis::AABBHasCollided(player_.GetRect(), pellet.GetRect())) {
			pellet.visible_ = false;
			IncrementPelletCount();
		}
	}
}

void GameplayScene::OnEvent(Aegis::Event& event)
{
	auto key_event = dynamic_cast<Aegis::KeyEvent*>(&event);

	if (key_event && (key_event->action_ == AE_BUTTON_PRESS || key_event->action_ == AE_BUTTON_REPEAT)) {
		switch(key_event->key_){
			case AE_KEY_ESCAPE: Pause(); break;
			case AE_KEY_W:
			case AE_KEY_UP: MovePlayer({0, -1}); break;
			case AE_KEY_S:
			case AE_KEY_DOWN: MovePlayer({0, 1}); break;
			case AE_KEY_A:
			case AE_KEY_LEFT: MovePlayer({-1, 0}); break;
			case AE_KEY_D:
			case AE_KEY_RIGHT: MovePlayer({1, 0}); break;
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

void GameplayScene::MovePlayer(Aegis::Vec2 dir)
{
	if (player_.IsMoving()){
		player_.queued_movement_ = GetSlidingTargetTile(*tile_map_, player_.target_grid_index_, dir);
	}
	else{
		player_.MoveTo(GetSlidingTargetTile(*tile_map_, player_.GetGridIndex(), dir));
	}
}

void GameplayScene::ResetObjectPositions()
{
	player_.SetPosition(tile_map_->bruce_spawn_index_ * tile_map_->tile_size_);
	brutus_.SetPosition(tile_map_->brutus_spawn_index_ * tile_map_->tile_size_);
	bjorn_.SetPosition(tile_map_->bjorn_spawn_index_ * tile_map_->tile_size_);

	player_.target_grid_index_ = player_.GetGridIndex();
	player_.queued_movement_ = {-1, -1};

	countdown_label_->visible_ = true;
	countdown_.Start(3000);
	stopwatch_.Stop();
}

void GameplayScene::SetUpLevel()
{
	ResetObjectPositions();

	stopwatch_.Restart();

	for (auto& pellet : pellets_){
		pellet.visible_ = true;
	}

	num_lives_ = max_lives_;
	score_board_->SetNumLives(max_lives_);

	pellets_collected_ = 0;
	score_board_->SetPelletCount(pellets_collected_, total_pellets_);

	//pause for info dialog;
	paused_ = game_data_.first_time_playing_;
}

void GameplayScene::RemoveLife()
{
	--num_lives_;

	score_board_->SetNumLives(num_lives_);
	if (num_lives_ > 0) {
		Aegis::AudioPlayer::PlaySound(death_sfx_);
		ResetObjectPositions();
	}
	else {
		game_over_dialog_->visible_ = true;
		paused_ = true;
		Aegis::AudioPlayer::PlaySound(game_over_sfx_);
	}
}

void GameplayScene::IncrementPelletCount()
{
	Aegis::AudioPlayer::PlaySound(fish_sfx_, 80);
	++pellets_collected_;
	score_board_->SetPelletCount(pellets_collected_, total_pellets_);

	if (pellets_collected_ == total_pellets_){
		paused_ = true;
		Aegis::AudioPlayer::PlaySound(level_complete_sfx_);
		score_dialog_->Show(stopwatch_.GetTimeInSeconds());
	}
}
