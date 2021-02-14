#include "LevelEditorScene.h"

#include "GameplayScene.h"

#include <filesystem>
#include <iostream>
#include <fstream>

LevelEditorScene::LevelEditorScene(int level)
	:level_num_(level)                  
{
	font_ = Aegis::FontManager::Load("assets/fonts/WorkSans-Regular.ttf", 24);
	auto tex_atlas = Aegis::TextureManager::Load("assets/textures/tundra-tile-map.png");

	if (level == -1){
		//create empty level
		tile_map_ = std::make_unique<TileMap>(31, 21, 32, tex_atlas); 
	}
	else{
		tile_map_ = std::make_unique<TileMap>("assets/levels/level" + std::to_string(level) + ".txt", 32, tex_atlas);
	}

	bruce_sprite_ = Aegis::Sprite(tex_atlas, { 0, 96, 32, 32 });
	bruce_sprite_.position_ = tile_map_->bruce_spawn_index_ * 32;
	brutus_sprite_ = Aegis::Sprite(tex_atlas, { 32, 96, 32, 32 });
	brutus_sprite_.position_ = tile_map_->brutus_spawn_index_ * 32;
	bjorn_sprite_ = Aegis::Sprite(tex_atlas, { 64, 96, 32, 32 });
	bjorn_sprite_.position_ = tile_map_->bjorn_spawn_index_ * 32;

	//used to center tilemap within window
	camera_.SetPosition({-270, -24});

	ui_layer_ = std::make_unique<Aegis::UILayer>(); 
	auto ui_font = Aegis::FontManager::Load("assets/fonts/WorkSans-Regular.ttf", 20);
	ui_layer_->SetFont(ui_font);
	
	auto ground_tile_button = ui_layer_->AddWidget<Aegis::Button>(Aegis::AABB( 40, 70, 64, 64 ), "Ground");  
	auto ice_tile_button = ui_layer_->AddWidget<Aegis::Button>(Aegis::AABB( 110, 70, 64, 64 ), " Ice");  
	auto wall_tile_button = ui_layer_->AddWidget<Aegis::Button>(Aegis::AABB( 180, 70, 64, 64 ), "Wall");  
	ground_tile_button->ConnectSignal("pressed", [&](){ChangeSelectedTile(tile_map_->tiles_map_['g']);});
	ice_tile_button->ConnectSignal("pressed", [&](){ChangeSelectedTile(tile_map_->tiles_map_['i']);});
	wall_tile_button->ConnectSignal("pressed", [&](){ChangeSelectedTile(tile_map_->tiles_map_['w']);});

	tile_text_ = ui_layer_->AddWidget<Aegis::Label>("Tile: None", Aegis::Vec2(40, 144));

	auto bjorn_button = ui_layer_->AddWidget<Aegis::Button>(Aegis::AABB( 40, 175, 64, 64 ), "Bjorn");  
	auto player_button = ui_layer_->AddWidget<Aegis::Button>(Aegis::AABB( 110, 175, 64, 64 ), "Bruce");  
	auto brutus_button = ui_layer_->AddWidget<Aegis::Button>(Aegis::AABB( 180, 175, 64, 64 ), "Brutus");  
	bjorn_button->ConnectSignal("pressed", [&](){ChangeSelectedSpawn(SpawnPoint::Bjorn);});
	player_button->ConnectSignal("pressed", [&](){ChangeSelectedSpawn(SpawnPoint::Bruce);});
	brutus_button->ConnectSignal("pressed", [&](){ChangeSelectedSpawn(SpawnPoint::Brutus);});
	spawn_text_ = ui_layer_->AddWidget<Aegis::Label>("Spawn: None", Aegis::Vec2(40, 249));

	auto undo_button = ui_layer_->AddWidget<Aegis::Button>(Aegis::AABB( 50, 400, 80, 40 ), "Undo");
	auto reset_button = ui_layer_->AddWidget<Aegis::Button>(Aegis::AABB( 140, 400, 80, 40 ), "Reset");
	undo_button->ConnectSignal("pressed", [&](){Undo();});
	reset_button->ConnectSignal("pressed", [&, tex_atlas]() {tile_map_->Clear(); });

	auto preview_button = ui_layer_->AddWidget<Aegis::Button>(Aegis::AABB(  50, 450, 80, 40  ), "Preview");
	auto save_button = ui_layer_->AddWidget<Aegis::Button>(Aegis::AABB( 140, 450, 80, 40 ), "Save");
	auto back_button = ui_layer_->AddWidget<Aegis::Button>(Aegis::AABB( 70, 500, 125, 40 ), "Exit");  
	preview_button->ConnectSignal("pressed", [&](){PreviewLevel();});
	save_button->ConnectSignal("pressed", [&](){SaveLevel();});
	back_button->ConnectSignal("pressed", [&](){manager_->PopScene();});
}

LevelEditorScene::~LevelEditorScene()
{
}

void LevelEditorScene::OnEvent(Aegis::Event& event)
{
	auto mouse_click = dynamic_cast<Aegis::MouseClickEvent*>(&event);
	if (mouse_click){
		if (mouse_click->action_ == AE_BUTTON_PRESS){
			recording_edits_ = true;
		}
		else{
			recording_edits_ = false;
			if (!recorded_edits_.empty()){
				edit_stack_.push(recorded_edits_);
			}

			while (!recorded_edits_.empty()){
				recorded_edits_.pop();
			}
		}
	}
	
	//only occurs when mouse button is being pressed
	if (recording_edits_){
		
		//have to substract camera position otherwise mouse_pos is off
		auto mouse_pos = Aegis::Application::GetWindow().GetMousePos() - Aegis::Vec2(270, 24);
		auto tile = tile_map_->GetTileByPos(mouse_pos.x, mouse_pos.y);

		if (tile != nullptr) {

			if (show_error_msg_) show_error_msg_ = false;

			auto index = tile_map_->GetGridIndexByPos(mouse_pos);
			if (selected_spawn_ != SpawnPoint::None){
				//spawns can not be on walls or on top of other entities
				if (tile_map_->bruce_spawn_index_ != index && tile_map_->bjorn_spawn_index_ != index && tile_map_->bruce_spawn_index_ != index){
					if (!tile->is_solid_){
						auto command = std::shared_ptr<EditCommand>(new SpawnEditCommand(*tile_map_, selected_spawn_, index));
						command->Execute();
						recorded_edits_.push(command);

						UpdateObjectPositions();
					}
				}
			}
			else if (selected_tile_ && selected_tile_ != tile){
				auto command = std::shared_ptr<EditCommand>(new TileEditCommand(*tile_map_, index, *selected_tile_));
				command->Execute();
				recorded_edits_.push(command);
			}
		}
	}
}

void LevelEditorScene::Update()
{

}

void LevelEditorScene::Render(float delta_time)
{
	Aegis::Renderer2D::SetProjection(camera_.view_projection_matrix_);
	Aegis::Renderer2D::SetFont(font_);
	tile_map_->Render();

	bruce_sprite_.Draw();
	brutus_sprite_.Draw();
	bjorn_sprite_.Draw();
	
	if (show_error_msg_){
		Aegis::DrawQuad({200, 300}, {675, 55}, {1.0, 1.0, 1.0, 0.8});
		Aegis::DrawText("Invalid Level.", {400, 300}, {1.0, 0.1, 0.1, 1.0});
		Aegis::DrawText("Bruce must be able to reach all ice tiles and both bears.", {200, 330}, {1.0, 0.1, 0.1, 1.0});
	}
}

bool LevelEditorScene::IsLevelValid()
{
	//All Ice tiles and bears must be reachable by player to be considered valid
	auto reachable_indices = tile_map_->GetReachableTileIndices(tile_map_->bruce_spawn_index_);
	if (!reachable_indices[tile_map_->brutus_spawn_index_.x][tile_map_->brutus_spawn_index_.y]){
		return false;
	}
	if (!reachable_indices[tile_map_->bjorn_spawn_index_.x][tile_map_->bjorn_spawn_index_.y]){
		return false;
	}

	for (int i = 0; i < tile_map_->grid_size_.x; ++i){
		for (int j = 0; j < tile_map_->grid_size_.y; ++j){
			if (tile_map_->GetTileByIndex(i, j)->is_slippery_){
				if (!reachable_indices[i][j]){
					return false;
				}
			}
		}
	}

	return true;
}

void LevelEditorScene::PreviewLevel()
{
	if (IsLevelValid()){
		manager_->PushScene(std::unique_ptr<Scene>(new GameplayScene(tile_map_)));
	}
	else{
		show_error_msg_ = true;
	}
}

void LevelEditorScene::SaveLevel()
{
	if (!IsLevelValid()){
		show_error_msg_ = true;
		return;
	}
	else{
		tile_map_->Save(level_num_);
	}
}

void LevelEditorScene::Undo()
{
	if (edit_stack_.empty()) return;

	else{
		auto recorded_commands = edit_stack_.top();
		edit_stack_.pop();

		while(!recorded_commands.empty()){
			recorded_commands.top()->Undo();
			recorded_commands.pop();
			
		}

		UpdateObjectPositions();
	}
}

void LevelEditorScene::ChangeSelectedTile(const Tile& tile)
{
	selected_tile_ = &tile; 
	selected_spawn_ = SpawnPoint::None;
	spawn_text_->SetText("Spawn: None");

	if (tile.is_solid_) tile_text_->SetText("Tile: Wall");
	else if (tile.is_slippery_) tile_text_->SetText("Tile: Ice");
	else tile_text_->SetText("Tile: Ground");
}

void LevelEditorScene::ChangeSelectedSpawn(SpawnPoint spawn)
{
	selected_spawn_ = spawn; 
	selected_tile_ = nullptr;
	tile_text_->SetText("Tile: None");

	switch (spawn){
		case SpawnPoint::Bjorn: spawn_text_->SetText("Spawn: Bjorn"); break;
		case SpawnPoint::Bruce: spawn_text_->SetText("Spawn: Bruce"); break;
		case SpawnPoint::Brutus: spawn_text_->SetText("Spawn: Brutus"); break;
		case SpawnPoint::None: break;
	}
}

void LevelEditorScene::UpdateObjectPositions()
{
	bjorn_sprite_.position_ = tile_map_->bjorn_spawn_index_ * 32;
	brutus_sprite_.position_ = tile_map_->brutus_spawn_index_ * 32;
	bruce_sprite_.position_ = tile_map_->bruce_spawn_index_ * 32;
}
