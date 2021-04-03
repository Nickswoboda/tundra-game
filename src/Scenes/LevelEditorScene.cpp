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
	font_ = Aegis::FontManager::Load("assets/fonts/worksans_regular.ttf", 24);
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

	bruce_sprite_ = Aegis::Sprite(tex_atlas_, { 0, 96, 32, 32 });
	bruce_sprite_.position_ = tile_map_->bruce_spawn_index_ * 32;
	brutus_sprite_ = Aegis::Sprite(tex_atlas_, { 32, 96, 32, 32 });
	brutus_sprite_.position_ = tile_map_->brutus_spawn_index_ * 32;
	bjorn_sprite_ = Aegis::Sprite(tex_atlas_, { 64, 96, 32, 32 });
	bjorn_sprite_.position_ = tile_map_->bjorn_spawn_index_ * 32;

	//used to center tilemap within window
	camera_.SetPosition({-270, -24});

	ui_layer_ = std::make_unique<Aegis::UILayer>(); 
	auto ui_font = Aegis::FontManager::Load("assets/fonts/worksans_regular.ttf", 20);
	ui_layer_->SetFont(ui_font);
	
	auto undo_button = ui_layer_->AddWidget<Aegis::Button>(Aegis::AABB( 50, 400, 80, 40 ), "Undo");
	auto reset_button = ui_layer_->AddWidget<Aegis::Button>(Aegis::AABB( 140, 400, 80, 40 ), "Reset");
	undo_button->ConnectSignal("pressed", [&](){Undo();});
	reset_button->ConnectSignal("pressed", [&]() {tile_map_->Clear(); });

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

void LevelEditorScene::OnEvent(Aegis::Event& event)
{
	auto mouse_click = dynamic_cast<Aegis::MouseClickEvent*>(&event);
	auto mouse_move = dynamic_cast<Aegis::MouseMoveEvent*>(&event);

	if (!mouse_click && !mouse_move){
		return;
	}

	//using GetMousePos because it takes resolution into account
	//have to substract camera position otherwise mouse_pos is off
	auto mouse_pos = Aegis::Application::GetWindow().GetMousePos() - Aegis::Vec2(270, 24);
	const Tile* tile = tile_map_->GetTileByPos(mouse_pos.x, mouse_pos.y);

	if (!tile){
		return;
	}

	auto tile_index = tile_map_->GetGridIndexByPos(mouse_pos);

	if (mouse_click){
		if (show_error_msg_) show_error_msg_ = false;

		recording_edits_ = true;
		if (mouse_click->action_ == AE_MOUSE_DOUBLE_PRESS && mouse_click->button_ == AE_MOUSE_BUTTON_LEFT){
			auto command = std::make_shared<TileEditCommand>(*tile_map_, tile_index, tile_map_->tiles_map_['g']);
			command->Execute();
			recorded_edits_.push(command);
		} 
		if (mouse_click->action_ == AE_BUTTON_RELEASE || mouse_click->action_ == AE_MOUSE_DOUBLE_PRESS){
			recording_edits_ = false;
			if (!recorded_edits_.empty()){
				edit_stack_.push(recorded_edits_);
			}

			while (!recorded_edits_.empty()){
				recorded_edits_.pop();
			}
			return;
		} 

		selected_tile_ = nullptr;

		if (mouse_click->action_ == AE_BUTTON_PRESS){
			if (mouse_click->button_ == AE_MOUSE_BUTTON_RIGHT){
				auto command = std::make_shared<FishEditCommand>(*tile_map_, tile_index);
				command->Execute();
				recorded_edits_.push(command);
			} else if (mouse_click->button_ == AE_MOUSE_BUTTON_LEFT){
				char new_tile_token = tile->is_slippery_ ? 'w' : 'i';
				selected_tile_ = &tile_map_->tiles_map_[new_tile_token]; 
			}
		}
	}
	
	if (recording_edits_){
		if (selected_tile_ && selected_tile_ != tile){
			auto command = std::shared_ptr<EditCommand>(new TileEditCommand(*tile_map_, tile_index, *selected_tile_));
			command->Execute();
			recorded_edits_.push(command);
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

	for (auto& index : tile_map_->pellet_spawn_indices_) {
		Aegis::DrawSubTexture(index * 32, {16, 16}, *tex_atlas_, fish_texture_coords_);
	}
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
		manager_->PushScene<GameplayScene>(tile_map_, game_data_);
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

void LevelEditorScene::UpdateObjectPositions()
{
	bjorn_sprite_.position_ = tile_map_->bjorn_spawn_index_ * 32;
	brutus_sprite_.position_ = tile_map_->brutus_spawn_index_ * 32;
	bruce_sprite_.position_ = tile_map_->bruce_spawn_index_ * 32;
}
