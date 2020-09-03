#include "LevelEditorScene.h"

#include "GameplayScene.h"

#include <filesystem>
#include <iostream>
#include <fstream>

LevelEditorScene::LevelEditorScene()
{
	font_ = Aegis::FontManager::Load("assets/fonts/WorkSans-Regular.ttf", 24);
	tile_map_ = std::make_unique<TileMap>(31, 21, 32); 

	//used to center tilemap within window
	camera_.SetPosition({-270, -24});

	ui_layer_ = std::make_unique<Aegis::UILayer>();
	ui_layer_->SetFont(Aegis::FontManager::Load("assets/fonts/WorkSans-Regular.ttf", 20));
	
	auto ground_tile_button = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({40, 70, 64, 64}, "Ground", [&](){ChangeSelectedTile(Tile::Ground);}));  
	auto ice_tile_button = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({110, 70, 64, 64}, " Ice", [&](){ChangeSelectedTile(Tile::Ice);}));  
	auto wall_tile_button = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({180, 70, 64, 64}, "Wall", [&](){ChangeSelectedTile(Tile::Wall);}));  

	auto bjorn_button = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({40, 175, 64, 64}, "Bjorn", [&](){ChangeSelectedSpawn(SpawnPoint::Bjorn);}));  
	auto player_button = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({110, 175, 64, 64}, "Bruce", [&](){ChangeSelectedSpawn(SpawnPoint::Bruce);}));  
	auto brutus_button = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({180, 175, 64, 64}, "Brutus", [&](){ChangeSelectedSpawn(SpawnPoint::Brutus);}));  

	auto undo_button = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({50, 400, 80, 40}, "Undo", [&]() {Undo();}));
	auto reset_button = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({140, 400, 80, 40}, "Reset", [&]() {tile_map_ = std::make_unique<TileMap>(31, 21, 32);}));

	auto preview_button = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({ 50, 450, 80, 40 }, "Preview", [&]() {PreviewLevel(); }));
	auto save_button = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({140, 450, 80, 40}, "Save", [&]() {SaveLevel();}));

	auto back_button = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({70, 500, 125, 40}, "Exit", [&](){ manager_->PopScene();}));  

	auto tex_atlas = Aegis::Texture::Create("assets/textures/tundra-tile-map.png");
	bruce_tex_ = std::make_shared<Aegis::SubTexture>(tex_atlas, Aegis::Vec2(96, 0), Aegis::Vec2(32, 32)); 
	brutus_tex_ =  std::make_shared<Aegis::SubTexture>(tex_atlas, Aegis::Vec2(128, 0), Aegis::Vec2(32, 32)); 
	bjorn_tex_ = std::make_shared<Aegis::SubTexture>(tex_atlas, Aegis::Vec2(160, 0), Aegis::Vec2(32, 32)); 
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

			auto index = tile_map_->GetTileIndex(*tile);
			if (selected_spawn_ != SpawnPoint::None){
				//spawns can not be on walls or on top of other entities
				if (tile_map_->bruce_spawn_index_ != index && tile_map_->bjorn_spawn_index_ != index && tile_map_->bruce_spawn_index_ != index){
					if (tile->type_ != Tile::Wall){
						auto command = std::shared_ptr<EditCommand>(new SpawnEditCommand(*tile_map_, selected_spawn_, index));
						command->Execute();
						recorded_edits_.push(command);
					}
				}
			}
			else if (selected_tile_ != tile->type_){
				auto command = std::shared_ptr<EditCommand>(new TileEditCommand(*tile, selected_tile_));
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
	Aegis::RendererClear();
	Aegis::Renderer2D::SetFont(font_);
	tile_map_->Render();

	//have to use negative numbers to counteract camera movement
	//TODO: add ability to submit text to UILayer
	switch (selected_tile_){
		case Tile::Wall: Aegis::DrawText("Tile: Wall", {-230, 120}); break;
		case Tile::Ice: Aegis::DrawText("Tile: Ice", {-230, 120}); break;
		case Tile::Ground: Aegis::DrawText("Tile: Ground", {-230, 120}); break;
		case Tile::NumTypes: Aegis::DrawText("Tile: None", {-230, 120}); break;
	}
	switch (selected_spawn_){
		case SpawnPoint::Bjorn: Aegis::DrawText("Spawn: Bjorn", {-230, 225}); break;
		case SpawnPoint::Brutus: Aegis::DrawText("Spawn: Brutus", {-230, 225}); break;
		case SpawnPoint::Bruce: Aegis::DrawText("Spawn: Bruce", {-230, 225}); break;
		case SpawnPoint::None: Aegis::DrawText("Spawn: None", {-230, 225}); break;
	}

	Aegis::DrawQuad(tile_map_->bruce_spawn_index_ * 32, {32, 32}, bruce_tex_);
	Aegis::DrawQuad(tile_map_->brutus_spawn_index_ * 32, {32, 32}, brutus_tex_);
	Aegis::DrawQuad(tile_map_->bjorn_spawn_index_ * 32, {32, 32}, bjorn_tex_);
	
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
			if (tile_map_->GetTileByIndex(i, j)->type_ == Tile::Ice){
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
		tile_map_->Save();
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
	}
}
