#include "LevelEditorScene.h"

#include "../Gameplay/GameplayScene.h"
#include "../LevelSelect/LevelSelectScene.h"
#include "../../PathFinding.h"
#include "../../Utilities.h"

#include <filesystem>
#include <iostream>
#include <fstream>


LevelEditorScene::LevelEditorScene(GameData& game_data, int level, bool is_custom)
	:level_num_(level), game_data_(game_data)
{
	auto sprite_sheet_ = Aegis::TextureManager::Load("assets/textures/tile_map.png");
	if (level == -1){
		//create empty level
		tile_map_ = std::make_unique<TileMap>(31, 21, 32, sprite_sheet_); 
	}
	else{
		std::string prefix = is_custom ? "assets/levels/custom_level_" : "assets/levels/level_";
		tile_map_ = std::make_unique<TileMap>(prefix + std::to_string(level) + ".txt", 32, sprite_sheet_);
	}

	level_editor_ = std::make_unique<LevelEditor>(*tile_map_);

	camera_.SetPosition({-270, -24});
	bg_texture_ = Aegis::TextureManager::Load("assets/textures/scene_bg.png");
	//used to center tilemap within window

	ui_layer_ = std::make_unique<Aegis::UILayer>(); 
	
	error_dialog_ = ui_layer_->AddWidget<EditorErrorDialog>();
	controls_dialog_ = ui_layer_->AddWidget<EditorControlsDialog>();

	Aegis::AABB rect{0, 0, 265, 600};
	Aegis::CenterAABBVertically(rect, Aegis::Application::GetWindow().GetViewport());
	auto button_box = ui_layer_->AddWidget<Aegis::Container>(rect, Aegis::Container::Vertical, 2, Aegis::Alignment::Center);

	first_star_time_ = button_box->AddWidget<Aegis::SpinBox>(120, 10);
	second_star_time_ = button_box->AddWidget<Aegis::SpinBox>(60, 10);
	auto editor_buttons_box = button_box->AddWidget<Aegis::Container>(Aegis::AABB(50, 300, 150, 120), Aegis::Container::Vertical, 2);
	auto undo_button = editor_buttons_box->AddWidget<Aegis::Button>(Aegis::AABB( 50, 400, 150, 50 ), "Undo");
	auto reset_button = editor_buttons_box->AddWidget<Aegis::Button>(Aegis::AABB( 140, 400, 150, 50 ), "Reset");
	undo_button->ConnectSignal("pressed", [&](){level_editor_->Undo();});
	reset_button->ConnectSignal("pressed", [&]() {level_editor_->ResetTileMap();});


	auto validity_buttons_box = button_box->AddWidget<Aegis::Container>(Aegis::AABB(50, 300, 150, 120), Aegis::Container::Vertical, 2);
	auto preview_button = validity_buttons_box->AddWidget<Aegis::Button>(Aegis::AABB(  50, 450, 150, 50  ), "Preview");
	auto save_button = validity_buttons_box->AddWidget<Aegis::Button>(Aegis::AABB( 140, 450, 150, 50 ), "Save");
	auto controls_button = button_box->AddWidget<Aegis::Button>(Aegis::AABB( 70, 500, 150, 50 ), "Controls");  
	auto back_button = button_box->AddWidget<Aegis::Button>(Aegis::AABB( 70, 500, 150, 50 ), "Exit");  
	preview_button->ConnectSignal("pressed", [&](){PreviewLevel();});
	save_button->ConnectSignal("pressed", [&](){SaveLevel();});
	controls_button->ConnectSignal("pressed", [&](){controls_dialog_->visible_ = true;});
	back_button->ConnectSignal("pressed", [&](){manager_->ReplaceScene<LevelSelectScene>(game_data, true);});

	StylizeButton(*undo_button, 3, 16);
	StylizeButton(*reset_button, 3, 16);
	StylizeButton(*preview_button, 3, 16);
	StylizeButton(*save_button, 3, 16);
	StylizeButton(*controls_button, 3, 16);
	StylizeButton(*back_button, 3, 16);

}

void LevelEditorScene::OnEvent(Aegis::Event& event)
{
	if (error_dialog_->visible_ || controls_dialog_->visible_) return;

	auto key_event = dynamic_cast<Aegis::KeyEvent*>(&event);
	if (key_event && key_event->action_ == AE_BUTTON_RELEASE){
		level_editor_->OnKeyRelease(key_event->key_);
		return;
	}

	auto mouse_click = dynamic_cast<Aegis::MouseClickEvent*>(&event);
	if (mouse_click){
		level_editor_->OnMouseClick(mouse_click->action_, mouse_click->button_);
		return;
	}

	auto mouse_move = dynamic_cast<Aegis::MouseMoveEvent*>(&event);
	if (mouse_move){
		level_editor_->OnMouseMove();
	}
}

void LevelEditorScene::Update()
{
}

void LevelEditorScene::Render(float delta_time)
{
	Aegis::Renderer2D::SetProjection(camera_.view_projection_matrix_);
	Aegis::DrawQuad({-270,-24}, *bg_texture_);
	level_editor_->Render(delta_time);
}

Error LevelEditorScene::IsLevelValid()
{
	//1. Allows for proper pathfinding for bears
	auto bruce_index = tile_map_->spawn_indices_[SpawnPoint::Bruce];
	auto brutus_index = tile_map_->spawn_indices_[SpawnPoint::Brutus];
	
	std::string error_msg;
	if (GetTargetTileCoordBFS(*tile_map_, brutus_index, bruce_index, false) == brutus_index){
		return Error::PathFinding;
	}
	auto bjorn_index = tile_map_->spawn_indices_[SpawnPoint::Bjorn];
	if (GetTargetTileCoordBFS(*tile_map_, bjorn_index, bruce_index, true) == bjorn_index){
		return Error::PathFinding;
	}
	//2. All fish are reachable
	if (tile_map_->pellet_spawn_indices_.empty()){
		return Error::NoFish;
	}

	for (const auto& index : tile_map_->pellet_spawn_indices_){
		if (GetTargetTileCoordBFS(*tile_map_, bruce_index, index, true) == bruce_index){
			return Error::FishReachability;
		}
	}

	return Error::None;
}

void LevelEditorScene::PreviewLevel()
{
	Error error = IsLevelValid();
	if (error == Error::None){
		manager_->PushScene<GameplayScene>(tile_map_, game_data_);
	} else {
		error_dialog_->Show(error);
	}
}

void LevelEditorScene::SaveLevel()
{
	Error error = IsLevelValid();
	if (error == Error::None){
		tile_map_->Save(level_num_);
		++game_data_.num_custom_levels_;
		std::array<int, 2> star_times = {first_star_time_->GetValue(), second_star_time_->GetValue()}; 
		game_data_.custom_star_thresholds_.push_back(star_times);
	} else {
		error_dialog_->Show(error);
	}
}

