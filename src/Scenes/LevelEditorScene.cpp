#include "LevelEditorScene.h"

#include "GameplayScene.h"
#include "LevelSelectScene.h"

#include <filesystem>
#include <iostream>
#include <fstream>

Aegis::Vec4 GetFishTextureCoords(const Aegis::Texture& texture)
{
	Aegis::AABB subtex_rect = {96, 96, 32, 32};
	float x1 = subtex_rect.pos.x / texture.size_.x;
	float y1 = subtex_rect.pos.y / texture.size_.y;
	float x2 = (subtex_rect.pos.x + subtex_rect.size.x) / texture.size_.x;
	float y2 = (subtex_rect.pos.y + subtex_rect.size.y) / texture.size_.y;
	return { x1, y1, x2, y2 };
}

LevelEditorScene::LevelEditorScene(GameData& game_data, int level, bool is_custom)
	:level_num_(level), game_data_(game_data)
{
	tex_atlas_ = Aegis::TextureManager::Load("assets/textures/tile_map.png");
	fish_texture_coords_ = GetFishTextureCoords(*tex_atlas_);

	if (level == -1){
		//create empty level
		tile_map_ = std::make_unique<TileMap>(31, 21, 32, tex_atlas_); 
	}
	else{
		std::string prefix = is_custom ? "assets/levels/custom_level_" : "assets/levels/level_";
		tile_map_ = std::make_unique<TileMap>(prefix + std::to_string(level) + ".txt", 32, tex_atlas_);
	}


	sprites_[SpawnPoint::Bruce] = Aegis::Sprite(tex_atlas_, { 0, 96, 32, 32 });
	sprites_[SpawnPoint::Brutus] = Aegis::Sprite(tex_atlas_, { 32, 96, 32, 32 });
	sprites_[SpawnPoint::Bjorn] = Aegis::Sprite(tex_atlas_, { 64, 96, 32, 32 });
	UpdateObjectPositions();
	//used to center tilemap within window
	camera_.SetPosition({-270, -24});

	ui_layer_ = std::make_unique<Aegis::UILayer>(); 
	auto ui_font = Aegis::FontManager::Load("assets/fonts/worksans_regular.ttf", 20);
	ui_layer_->SetFont(ui_font);
	
	auto undo_button = ui_layer_->AddWidget<Aegis::Button>(Aegis::AABB( 50, 400, 80, 40 ), "Undo");
	auto reset_button = ui_layer_->AddWidget<Aegis::Button>(Aegis::AABB( 140, 400, 80, 40 ), "Reset");
	undo_button->ConnectSignal("pressed", [&](){Undo();});
	reset_button->ConnectSignal("pressed", [&]() {tile_map_->Clear(); UpdateObjectPositions();});

	auto preview_button = ui_layer_->AddWidget<Aegis::Button>(Aegis::AABB(  50, 450, 80, 40  ), "Preview");
	auto save_button = ui_layer_->AddWidget<Aegis::Button>(Aegis::AABB( 140, 450, 80, 40 ), "Save");
	auto back_button = ui_layer_->AddWidget<Aegis::Button>(Aegis::AABB( 70, 500, 125, 40 ), "Exit");  
	preview_button->ConnectSignal("pressed", [&](){PreviewLevel();});
	save_button->ConnectSignal("pressed", [&](){SaveLevel();});
	back_button->ConnectSignal("pressed", [&](){manager_->ReplaceScene<LevelSelectScene>(game_data, true);});
}

LevelEditorScene::~LevelEditorScene()
{
}

bool LevelEditorScene::RequestEdit(EditType edit_type, const char new_tile_token)
{
	auto mouse_pos = Aegis::Application::GetWindow().GetMousePos() - Aegis::Vec2{270, 24};
	const Tile* tile = tile_map_->GetTileByPos(mouse_pos.x, mouse_pos.y);

	if (!tile) return false;
	Aegis::Vec2 tile_index = tile_map_->GetGridIndexByPos(mouse_pos);

	std::shared_ptr<EditCommand> command;
	switch (edit_type) {
		case EditType::Tile: {
			if (new_tile_token != 'g'){
				selected_tile_token = new_tile_token;
				recording_edits_ = true;
			}
			auto& new_tile = tile_map_->tiles_map_[new_tile_token];
			command = std::make_shared<TileEditCommand>(*tile_map_, tile_index, new_tile);
			break;
		}
		case EditType::Spawn: { 
			if (!grabbed_sprite_){
				return (GrabEntityAtIndex(tile_index));
			}
			if (tile->is_slippery_){
				command = std::make_shared<SpawnEditCommand>(*tile_map_, spawn_being_edited_, tile_index);
			}
			break;
		}
		case EditType::Fish: { 
			if (tile->is_slippery_){
				command = std::make_shared<FishEditCommand>(*tile_map_, tile_index);
			}
			break;
		}
	}

	if (command){
		command->Execute();
		recorded_edits_.push(command);
		return true;
	} else {
		return false;
	}
}

void LevelEditorScene::OnEvent(Aegis::Event& event)
{
	auto key_event = dynamic_cast<Aegis::KeyEvent*>(&event);
	if (key_event && !recording_edits_){
		if (key_event->action_ == AE_BUTTON_RELEASE){
			if (key_event->key_ == AE_KEY_F){
				RequestEdit(EditType::Fish);
				PushEditsOntoStack();
			}
			if (key_event->key_ == AE_KEY_G){
				RequestEdit(EditType::Tile, 'g');
				PushEditsOntoStack();
			}
		}
		return;
	}

	auto mouse_click = dynamic_cast<Aegis::MouseClickEvent*>(&event);
	if (mouse_click){
		if (mouse_click->action_ == AE_BUTTON_RELEASE){
			if (grabbed_sprite_){
				RequestEdit(EditType::Spawn);
				UpdateObjectPositions();
			}
			PushEditsOntoStack();
		} 

		if (mouse_click->action_ == AE_BUTTON_PRESS){
			if (mouse_click->button_ == AE_MOUSE_BUTTON_RIGHT){
				RequestEdit(EditType::Tile, 'w');
			} else if (mouse_click->button_ == AE_MOUSE_BUTTON_LEFT){
				if (!RequestEdit(EditType::Spawn)){
					RequestEdit(EditType::Tile, ('i'));
				}
			}
		}
		return;
	}

	auto mouse_move = dynamic_cast<Aegis::MouseMoveEvent*>(&event);
	if (mouse_move){
		if (grabbed_sprite_){
			MoveGrabbedSprite();
		} else if (recording_edits_){
			RequestEdit(EditType::Tile, selected_tile_token);
		}
	}
}

void LevelEditorScene::Update()
{

}

void LevelEditorScene::Render(float delta_time)
{
	Aegis::Renderer2D::SetProjection(camera_.view_projection_matrix_);
	tile_map_->Render();
	tile_map_->DrawGridLines();

	for (const auto& [spawn, sprite] : sprites_){
		sprite.Draw();
	}

	for (auto& index : tile_map_->pellet_spawn_indices_) {
		Aegis::DrawSubTexture(index * 32, {16, 16}, *tex_atlas_, fish_texture_coords_);
	}
}

void LevelEditorScene::PushEditsOntoStack()
{
	if (!recorded_edits_.empty()){
		edit_stack_.push(recorded_edits_);
	}

	while (!recorded_edits_.empty()){
		recorded_edits_.pop();
	}
	recording_edits_ = false;
}

void LevelEditorScene::MoveGrabbedSprite()
{
	auto mouse_pos = Aegis::Application::GetWindow().GetMousePos() - Aegis::Vec2{270, 24};
	grabbed_sprite_->position_ = mouse_pos - (grabbed_sprite_->GetRect().size / 2);
}

bool LevelEditorScene::GrabEntityAtIndex(Aegis::Vec2 index)
{
	for (const auto& [spawn, idx] : tile_map_->spawn_indices_){
		if (idx == index){
			grabbed_sprite_ = &sprites_[spawn];
			spawn_being_edited_ = spawn;

			MoveGrabbedSprite();
			return true;
		}
	}

	return false;
}

bool LevelEditorScene::IsLevelValid()
{
	//All Ice tiles and bears must be reachable by player to be considered valid
	auto reachable_indices = tile_map_->GetReachableTileIndices(tile_map_->spawn_indices_[SpawnPoint::Bruce]);
	auto brutus_index = tile_map_->spawn_indices_[SpawnPoint::Brutus];
	if (!reachable_indices[brutus_index.x][brutus_index.y]){
		return false;
	}
	auto bjorn_index = tile_map_->spawn_indices_[SpawnPoint::Bjorn];
	if (!reachable_indices[bjorn_index.x][bjorn_index.y]){
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
		manager_->PushScene<GameplayScene>(tile_map_, game_data_);
	}
	else{
		//show error dialog
	}
}

void LevelEditorScene::SaveLevel()
{
	if (IsLevelValid()){
		tile_map_->Save(level_num_);
	}
	else{
		//show error dialog
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

void LevelEditorScene::UpdateObjectPositions()
{
	grabbed_sprite_ = nullptr;

	for (auto& [spawn, sprite] : sprites_){
		sprite.position_ = tile_map_->spawn_indices_[spawn] * 32;
	}
}
