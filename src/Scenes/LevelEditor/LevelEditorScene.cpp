#include "LevelEditorScene.h"

#include "../Gameplay/GameplayScene.h"
#include "../LevelSelect/LevelSelectScene.h"
#include "../../PathFinding.h"

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

	//used to center tilemap within window
	camera_.SetPosition({-270, -24});

	ui_layer_ = std::make_unique<Aegis::UILayer>(); 
	
	error_dialog_ = ui_layer_->AddWidget<EditorErrorDialog>();

	auto undo_button = ui_layer_->AddWidget<Aegis::Button>(Aegis::AABB( 50, 400, 80, 40 ), "Undo");
	auto reset_button = ui_layer_->AddWidget<Aegis::Button>(Aegis::AABB( 140, 400, 80, 40 ), "Reset");
	undo_button->ConnectSignal("pressed", [&](){level_editor_->Undo();});
	reset_button->ConnectSignal("pressed", [&]() {level_editor_->ResetTileMap();});

	auto preview_button = ui_layer_->AddWidget<Aegis::Button>(Aegis::AABB(  50, 450, 80, 40  ), "Preview");
	auto save_button = ui_layer_->AddWidget<Aegis::Button>(Aegis::AABB( 140, 450, 80, 40 ), "Save");
	auto back_button = ui_layer_->AddWidget<Aegis::Button>(Aegis::AABB( 70, 500, 125, 40 ), "Exit");  
	preview_button->ConnectSignal("pressed", [&](){PreviewLevel();});
	save_button->ConnectSignal("pressed", [&](){SaveLevel();});
	back_button->ConnectSignal("pressed", [&](){manager_->ReplaceScene<LevelSelectScene>(game_data, true);});

}

void LevelEditorScene::OnEvent(Aegis::Event& event)
{
	if (error_dialog_->visible_) return;

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
	} else {
		error_dialog_->Show(error);
	}
}

