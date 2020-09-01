#include "LevelEditorScene.h"

#include "GameplayScene.h"

#include <filesystem>
#include <iostream>
#include <fstream>

LevelEditorScene::LevelEditorScene()
{
	font_ = Aegis::FontManager::Instance().Load("assets/fonts/WorkSans-Regular.ttf", 24);
	tile_map_ = std::make_unique<TileMap>(31, 21, 32); 
	//used to center tilemap within window
	camera_.SetPosition({-270, -24, 0});

	ui_layer_ = std::make_unique<Aegis::UILayer>();
	ui_layer_->SetFont(Aegis::FontManager::Instance().Load("assets/fonts/WorkSans-Regular.ttf", 20));
	
	auto ground_tile_button = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({40, 70, 64, 64}, "Ground", [&](){ChangeSelectedTile(Tile::Ground); button_highligh_pos_ = {50, 200};}));  
	auto ice_tile_button = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({110, 70, 64, 64}, " Ice", [&](){ChangeSelectedTile(Tile::Ice);}));  
	auto wall_tile_button = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({180, 70, 64, 64}, "Wall", [&](){ChangeSelectedTile(Tile::Wall);}));  

	auto bjorne_button = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({40, 175, 64, 64}, "Bjorne", [&](){ChangeSelectedSpawn(SpawnPoint::Bjorne);}));  
	auto player_button = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({110, 175, 64, 64}, "Bruce", [&](){ChangeSelectedSpawn(SpawnPoint::Bruce);}));  
	auto brutus_button = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({180, 175, 64, 64}, "Brutus", [&](){ChangeSelectedSpawn(SpawnPoint::Brutus);}));  

	auto undo_button = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({50, 400, 80, 40}, "Undo", [&]() {Undo();}));
	auto reset_button = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({140, 400, 80, 40}, "Reset", [&]() {tile_map_ = std::make_unique<TileMap>(31, 21, 32);}));

	auto preview_button = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({ 50, 450, 80, 40 }, "Preview", [&]() {PreviewLevel(); }));
	auto save_button = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({140, 450, 80, 40}, "Save", [&]() {SaveLevel();}));

	auto back_button = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({70, 500, 125, 40}, "BACK", [&](){ manager_->PopScene();}));  

	auto tex_atlas = Aegis::Texture::Create("assets/textures/tundra-tile-map.png");
	bruce_tex_ = std::make_shared<Aegis::SubTexture>(tex_atlas, Aegis::Vec2(96, 0), Aegis::Vec2(32, 32)); 
	brutus_tex_ =  std::make_shared<Aegis::SubTexture>(tex_atlas, Aegis::Vec2(128, 0), Aegis::Vec2(32, 32)); 
	bjorne_tex_ = std::make_shared<Aegis::SubTexture>(tex_atlas, Aegis::Vec2(160, 0), Aegis::Vec2(32, 32)); 
		
}

LevelEditorScene::~LevelEditorScene()
{
}

void LevelEditorScene::OnEvent(Aegis::Event& event)
{
	auto mouse_click = dynamic_cast<Aegis::MouseClickEvent*>(&event);
	if (mouse_click){
		if (mouse_click->action_ == AE_BUTTON_PRESS){
			recording_ = true;
		}
		else{
			recording_ = false;
			if (!recorded_edits_.empty()){
				command_stack_.push(recorded_edits_);
			}

			while (!recorded_edits_.empty()){
				recorded_edits_.pop();
			}
		}
	}
	if (recording_){
		
		//have to substract camera position otherwise mouse_pos is off
		auto mouse_pos = Aegis::Application::GetWindow().GetMousePos() - Aegis::Vec2(270, 24);
		auto tile = tile_map_->GetTileByPos(mouse_pos.x, mouse_pos.y);

		if (tile != nullptr) {

			if (show_error_msg_) show_error_msg_ = false;

			auto index = tile_map_->GetTileIndex(*tile);
			if (selected_spawn_ != SpawnPoint::None){
				//spawns can not be on walls or on top of other entities
				if (tile_map_->brutus_start_pos_ != index && tile_map_->bjorne_start_pos_ != index && tile_map_->player_start_pos_ != index){
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

	if (show_error_msg_){
		Aegis::DrawQuad({200, 300}, {675, 55}, {1.0, 1.0, 1.0, 0.8});
		Aegis::DrawText("Invalid Level.", {400, 300}, {1.0, 0.1, 0.1, 1.0});
		Aegis::DrawText("Bruce must be able to reach all ice tiles and both bears.", {200, 330}, {1.0, 0.1, 0.1, 1.0});
	}

	//have to use negative numbers to counteract camera movement
	//TODO: add ability to submit text to UILayer
	switch (selected_tile_){
		case Tile::Wall: Aegis::DrawText("Tile: Wall", {-230, 120}); break;
		case Tile::Ice: Aegis::DrawText("Tile: Ice", {-230, 120}); break;
		case Tile::Ground: Aegis::DrawText("Tile: Ground", {-230, 120}); break;
		case Tile::NumTypes: Aegis::DrawText("Tile: None", {-230, 120}); break;
	}
	switch (selected_spawn_){
		case SpawnPoint::Bjorne: Aegis::DrawText("Spawn: Bjorne", {-230, 225}); break;
		case SpawnPoint::Brutus: Aegis::DrawText("Spawn: Brutus", {-230, 225}); break;
		case SpawnPoint::Bruce: Aegis::DrawText("Spawn: Bruce", {-230, 225}); break;
		case SpawnPoint::None: Aegis::DrawText("Spawn: None", {-230, 225}); break;
	}

	Aegis::DrawQuad(tile_map_->player_start_pos_ * 32, {32, 32}, bruce_tex_);
	Aegis::DrawQuad(tile_map_->brutus_start_pos_ * 32, {32, 32}, brutus_tex_);
	Aegis::DrawQuad(tile_map_->bjorne_start_pos_ * 32, {32, 32}, bjorne_tex_);
}

std::vector<std::vector<int>> LevelEditorScene::GetReachableTileIndices(Aegis::Vec2 start_pos)
{
	std::vector<Aegis::Vec2> frontier;
	frontier.push_back(start_pos);
	std::vector<std::vector<int>> seen(tile_map_->grid_size_.x, std::vector<int>(tile_map_->grid_size_.y, 0));
	seen[start_pos.x][start_pos.y] = 1;
	while (!frontier.empty()) {
		auto current = frontier[0];
		frontier.erase(frontier.begin());

		Aegis::Vec2 up = current + Aegis::Vec2(0, -1);
		Aegis::Vec2 down = current + Aegis::Vec2(0, 1);
		Aegis::Vec2 left = current + Aegis::Vec2(-1, 0);
		Aegis::Vec2 right = current + Aegis::Vec2(1, 0);

		auto up_tile = tile_map_->GetTileByIndex(up.x, up.y);
		auto down_tile = tile_map_->GetTileByIndex(down.x, down.y);
		auto left_tile = tile_map_->GetTileByIndex(left.x, left.y);
		auto right_tile = tile_map_->GetTileByIndex(right.x, right.y);

		if (up_tile && up_tile->type_ != Tile::Wall) {
			if (seen[up.x][up.y] == 0){
				frontier.push_back(up);
				seen[up.x][up.y] = 1;
			}
		}
		if (down_tile && down_tile->type_ != Tile::Wall) {
			if (seen[down.x][down.y] == 0){
				frontier.push_back(down);
				seen[down.x][down.y] = 1;
			}
		}
		if (left_tile && left_tile->type_ != Tile::Wall) {
			if (seen[left.x][left.y] == 0){
				frontier.push_back(left);
				seen[left.x][left.y] = 1;
			}
		}
		if (right_tile && right_tile->type_ != Tile::Wall) {
			if (seen[right.x][right.y] == 0){
				frontier.push_back(right);
				seen[right.x][right.y] = 1;
			}
		}

	}
	return seen;
}

bool LevelEditorScene::IsLevelValid()
{
	//All Ice tiles and bears must be reachable by player to be considered valid
	auto reachable_indices = GetReachableTileIndices(tile_map_->player_start_pos_);
	if (!reachable_indices[tile_map_->brutus_start_pos_.x][tile_map_->brutus_start_pos_.y]){
		std::cout << "Invalid level\n";
		return false;
	}
	if (!reachable_indices[tile_map_->bjorne_start_pos_.x][tile_map_->bjorne_start_pos_.y]){
		std::cout << "Invalid level\n";
		return false;
	}

	for (int i = 0; i < tile_map_->grid_size_.x; ++i){
		for (int j = 0; j < tile_map_->grid_size_.y; ++j){
			if (tile_map_->GetTileByIndex(i, j)->type_ == Tile::Ice){
				if (!reachable_indices[i][j]){
					std::cout << "Invalid level\n";
					return false;
				}
			}
		}
	}
	std::cout << "valid\n";
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

	int level = 1;
	std::string new_file_path = "assets/levels/level" + std::to_string(level) + ".txt";

	while (std::filesystem::exists(new_file_path)) {
		++level;
		new_file_path = "assets/levels/level" + std::to_string(level) + ".txt";
	}
	std::ofstream file(new_file_path);

	for (int row = 0; row < tile_map_->grid_size_.y; ++row) {
		for (int col = 0; col < tile_map_->grid_size_.x; ++col) {
			auto coord = Aegis::Vec2(col, row);

			if (coord == tile_map_->brutus_start_pos_) {
				file << 'C';
			}
			else if (coord == tile_map_->player_start_pos_) {
				file << 'P';
			}
			else if (coord == tile_map_->bjorne_start_pos_) {
				file << 'B';
			}
			auto type = tile_map_->tiles_[col][row].type_;

			switch (type)
			{
			case Tile::Type::Wall: {file << '0'; break; }
			case Tile::Type::Ice: {file << '1'; break; }
			case Tile::Type::Ground: {file << ' '; break; }
			case Tile::Type::NumTypes: break;
			}
		}

		//don't print newline on last row
		if (row != tile_map_->grid_size_.y - 1) {
			file << '\n';
		}
	}
	

	file.close();
}

void LevelEditorScene::Undo()
{
	if (command_stack_.empty()) return;
	else{
		auto recorded_commands = command_stack_.top();
		command_stack_.pop();

		while(!recorded_commands.empty()){
			recorded_commands.top()->Undo();
			recorded_commands.pop();
			
		}
	}
}

TileEditCommand::TileEditCommand(Tile& tile, Tile::Type new_type)
	:tile_(tile), prev_type_(tile.type_), new_type_(new_type)
{
}

void TileEditCommand::Execute()
{
	switch (new_type_){
		case Tile::Ground: tile_ = Ground(tile_.pos_.x, tile_.pos_.y); break;
		case Tile::Ice: tile_ = Ice(tile_.pos_.x, tile_.pos_.y); break;
		case Tile::Wall: tile_ = Wall(tile_.pos_.x, tile_.pos_.y); break;
		case Tile::NumTypes: break;
	}
}
void TileEditCommand::Undo()
{
	switch (prev_type_){
		case Tile::Ground: tile_ = Ground(tile_.pos_.x, tile_.pos_.y); break;
		case Tile::Ice: tile_ = Ice(tile_.pos_.x, tile_.pos_.y); break;
		case Tile::Wall: tile_ = Wall(tile_.pos_.x, tile_.pos_.y); break;
		case Tile::NumTypes: break;
	}
}

SpawnEditCommand::SpawnEditCommand(TileMap& tile_map, SpawnPoint spawn_point, Aegis::Vec2 new_index)
	: tile_map_(tile_map), spawn_point_(spawn_point),  new_index_(new_index) 
{
	switch (spawn_point_){
		case SpawnPoint::Bjorne: prev_index_ = tile_map_.bjorne_start_pos_; break;
		case SpawnPoint::Brutus: prev_index_ = tile_map_.brutus_start_pos_; break;
		case SpawnPoint::Bruce: prev_index_ = tile_map_.player_start_pos_; break;
		case SpawnPoint::None: break;
	}
}

void SpawnEditCommand::Execute()
{
	switch (spawn_point_){
		case SpawnPoint::Bjorne: tile_map_.bjorne_start_pos_ = new_index_; break;
		case SpawnPoint::Brutus: tile_map_.brutus_start_pos_ = new_index_; break;
		case SpawnPoint::Bruce:  tile_map_.player_start_pos_ = new_index_; break;
		case SpawnPoint::None: break;
	}

}

void SpawnEditCommand::Undo()
{
	switch (spawn_point_){
		case SpawnPoint::Bjorne: tile_map_.bjorne_start_pos_ = prev_index_; break;
		case SpawnPoint::Brutus: tile_map_.brutus_start_pos_ = prev_index_; break;
		case SpawnPoint::Bruce:  tile_map_.player_start_pos_ = prev_index_; break;
		case SpawnPoint::None: break;
	}

}
