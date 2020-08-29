#include "LevelEditorScene.h"

#include <filesystem>
#include <fstream>

LevelEditorScene::LevelEditorScene()
{
	tile_map_ = std::make_unique<TileMap>("assets/levels/level1.txt", 32); 
	//used to center tilemap within window
	camera_.SetPosition({-270, -24, 0});
	
	ui_layer_ = std::make_unique<Aegis::UILayer>();
	ui_layer_->SetFont(Aegis::FontManager::Instance().Load("assets/fonts/WorkSans-Regular.ttf", 20));
	
	auto back_button = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({50, 600, 125, 40}, "BACK", [&](){ manager_->PopScene();}));  
	auto round_tile_button = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({50, 200, 32, 32}, "G", [&](){ChangeSelectedTile(Tile::Ground);}));  
	auto ice_tile_button = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({100, 200, 32, 32}, "I", [&](){ChangeSelectedTile(Tile::Ice);}));  
	auto wall_tile_button = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({150, 200, 32, 32}, "W", [&](){ChangeSelectedTile(Tile::Wall);}));  
	auto bjorne_button = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({50, 270, 32, 32}, "Bj", [&](){ChangeSelectedSpawn(SpawnPoint::Bjorne);}));  
	auto player_button = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({100, 270, 32, 32}, "Pl", [&](){ChangeSelectedSpawn(SpawnPoint::Bruce);}));  
	auto brutus_button = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({150, 270, 32, 32}, "Br", [&](){ChangeSelectedSpawn(SpawnPoint::Brutus);}));  
	auto save_button = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({50, 550, 125, 40}, "Save", [&]() {SaveLevel();}));
	auto undo_button = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({50, 500, 125, 40}, "Undo", [&]() {Undo();}));
	
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
	auto mouse_move = dynamic_cast<Aegis::MouseMoveEvent*>(&event);

	if ((mouse_click && mouse_click->action_) == AE_BUTTON_PRESS || (mouse_move && Aegis::Application::GetWindow().IsMousePressed())){
		
		//have to substract camera position otherwise mouse_pos is off
		auto mouse_pos = Aegis::Application::GetWindow().GetMousePos() - Aegis::Vec2(270, 24);
		auto tile = tile_map_->GetTileByPos(mouse_pos.x, mouse_pos.y);

		if (tile != nullptr) {
			auto index = tile_map_->GetTileIndex(*tile);
			Aegis::Vec2 tile_spawn_pos = index * tile_map_->tile_size_;

			if (selected_spawn_ != SpawnPoint::None){
				switch (selected_spawn_){
					case SpawnPoint::Bjorne: tile_map_->bjorne_start_pos_ = index; break;
					case SpawnPoint::Brutus:  tile_map_->brutus_start_pos_ = index;break;
					case SpawnPoint::Bruce:  tile_map_->player_start_pos_ = index; break;
				}
			}
			else if (selected_tile_ != tile->type_){
				undo_stack_.push({index, tile->type_});
				switch (selected_tile_){
					case Tile::Ground: *tile = Ground(tile_spawn_pos.x, tile_spawn_pos.y); break;
					case Tile::Ice: *tile = Ice(tile_spawn_pos.x, tile_spawn_pos.y); break;
					case Tile::Wall: *tile = Wall(tile_spawn_pos.x, tile_spawn_pos.y); break;
				}
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

	tile_map_->Render();

	//have to use negative numbers to counteract camera movement
	//TODO: add ability to submit text to UILayer
	Aegis::DrawText("Tiles:", {-200, 150});
	Aegis::DrawText("Spawns:", {-200, 220});

	Aegis::DrawQuad(tile_map_->player_start_pos_ * 32, {32, 32}, bruce_tex_);
	Aegis::DrawQuad(tile_map_->brutus_start_pos_ * 32, {32, 32}, brutus_tex_);
	Aegis::DrawQuad(tile_map_->bjorne_start_pos_ * 32, {32, 32}, bjorne_tex_);
}

void LevelEditorScene::SaveLevel()
{
	//if (IsValidLevel())

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
	if (undo_stack_.empty()) return;
	else{
		auto undo = undo_stack_.top();
		undo_stack_.pop();

		auto tile = tile_map_->GetTileByIndex(undo.tile_pos_.x, undo.tile_pos_.y);
		auto tile_spawn_pos = undo.tile_pos_ * tile_map_->tile_size_;
		switch (undo.type_){
			case Tile::Ground: *tile = Ground(tile_spawn_pos.x, tile_spawn_pos.y); break;
			case Tile::Ice: *tile = Ice(tile_spawn_pos.x, tile_spawn_pos.y); break;
			case Tile::Wall: *tile = Wall(tile_spawn_pos.x, tile_spawn_pos.y); break;
		}
	}
}
