#include "LevelEditorScene.h"

#include "../Gameplay/GameplayScene.h"
#include "../LevelSelect/LevelSelectScene.h"
#include "../../PathFinding.h"
#include "../../Utilities.h"

#include <filesystem>
#include <iostream>
#include <fstream>


LevelEditorScene::LevelEditorScene(GameData& game_data, int level)
	:level_num_(level), game_data_(game_data)
{
	auto sprite_sheet_ = Aegis::TextureManager::Load("assets/textures/tile_map.png");
	if (level == -1){
		//create empty level
		tile_map_ = std::make_unique<TileMap>(31, 21, 32, sprite_sheet_); 
	}
	else{
		std::string prefix = "assets/levels/custom_level_";
		tile_map_ = std::make_unique<TileMap>(prefix + std::to_string(level) + ".txt", 32, sprite_sheet_);
	}

	level_editor_ = std::make_unique<LevelEditor>(*tile_map_);

	//used to center tilemap within window
	camera_.SetPosition({-270, -24});
	bg_texture_ = Aegis::TextureManager::Load("assets/textures/scene_bg.png");

	ui_layer_ = std::make_unique<Aegis::UILayer>(); 
	
	popup_ = ui_layer_->AddWidget<PopUpDialog>();
	controls_dialog_ = ui_layer_->AddWidget<EditorControlsDialog>();

	Aegis::AABB rect{0, 0, 265, 600};
	Aegis::CenterAABBVertically(rect, Aegis::Application::GetWindow().GetViewport());
	auto button_box = ui_layer_->AddWidget<Aegis::VContainer>();
	button_box->SetPos(rect.pos);
	button_box->SetSize(rect.size);
	button_box->SetAlignment(Aegis::Alignment::Center);

	auto threshold_box = button_box->AddWidget<Aegis::VContainer>();
	threshold_box->SetSize({150, 150});
	threshold_box->SetAlignment(Aegis::Alignment::Center);

	auto label = threshold_box->AddWidget<Aegis::Label>("Star Thresholds:", Aegis::Vec2());
	label->SetFont(Aegis::FontManager::Load("assets/fonts/roboto_bold.ttf", 18));

	auto two_star_box = threshold_box->AddWidget<Aegis::HContainer>();
	two_star_box->SetSize({150, 50});
	two_star_box->SetPadding(0);
	two_star_box->SetAlignment(Aegis::Alignment::Center);

	auto two_sprite_box = two_star_box->AddWidget<Aegis::HContainer>();
	two_sprite_box->SetSize({50, 50});
	two_sprite_box->SetPadding(0);
	two_sprite_box->SetAlignment(Aegis::Alignment::Center);

	for (auto i = 0; i < 2; ++i){
		auto star = std::make_shared<Aegis::SpriteWidget>(Aegis::Vec2(), sprite_sheet_, Aegis::AABB(0, 128, 32, 32));
		star->SetScale({0.5f, 0.5f});
		two_sprite_box->AddWidget(star);
	}
	int second_star_time = level == -1 ? 120 : game_data_.custom_star_thresholds_[level-1][0];
	two_star_spinbox_ = two_star_box->AddWidget<Aegis::SpinBox>(second_star_time, 10);

	auto three_star_box = threshold_box->AddWidget<Aegis::HContainer>();
	three_star_box->SetSize({150, 50});
	three_star_box->SetPadding(0);
	three_star_box->SetAlignment(Aegis::Alignment::Center);

	auto three_sprite_box = three_star_box->AddWidget<Aegis::HContainer>();
	three_sprite_box->SetSize({50, 50});
	three_sprite_box->SetPadding(0);
	three_sprite_box->SetAlignment(Aegis::Alignment::Center);

	for (auto i = 0; i < 3; ++i){
		auto star = std::make_shared<Aegis::SpriteWidget>(Aegis::Vec2(), sprite_sheet_, Aegis::AABB(0, 128, 32, 32));
		star->SetScale({0.5f, 0.5f});
		three_sprite_box->AddWidget(star);
	}
	int third_star_time = level == -1 ? 60 : game_data_.custom_star_thresholds_[level-1][1];
	three_star_spinbox_ = three_star_box->AddWidget<Aegis::SpinBox>(third_star_time, 10);

	StylizeSpinBox(*two_star_spinbox_, 3, 16);
	StylizeSpinBox(*three_star_spinbox_, 3, 16);

	auto editor_buttons_box = button_box->AddWidget<Aegis::VContainer>();
	editor_buttons_box->SetSize({150, 120});

	auto undo_button = editor_buttons_box->AddWidget<Aegis::Button>("Undo");
	undo_button->SetSize({ 150, 50 });
	StylizeButton(*undo_button, 3, 16);
	undo_button->ConnectSignal("pressed", [&](){level_editor_->Undo();});

	auto reset_button = editor_buttons_box->AddWidget<Aegis::Button>( "Reset");
	reset_button->SetSize({ 150, 50 });
	StylizeButton(*reset_button, 3, 16);
	reset_button->ConnectSignal("pressed", [&]() {level_editor_->ResetTileMap();});

	auto validity_buttons_box = button_box->AddWidget<Aegis::VContainer>();
	validity_buttons_box->SetSize({150, 120});

	auto preview_button = validity_buttons_box->AddWidget<Aegis::Button>("Preview");
	preview_button->SetSize({ 150, 50 });
	StylizeButton(*preview_button, 3, 16);
	preview_button->ConnectSignal("pressed", [&](){PreviewLevel();});

	auto save_button = validity_buttons_box->AddWidget<Aegis::Button>("Save");
	save_button->SetSize({ 150, 50 });
	StylizeButton(*save_button, 3, 16);
	save_button->ConnectSignal("pressed", [&](){SaveLevel();});

	auto controls_button = button_box->AddWidget<Aegis::Button>("Controls");  
	controls_button->SetSize({ 150, 50 });
	StylizeButton(*controls_button, 3, 16);
	controls_button->ConnectSignal("pressed", [&](){controls_dialog_->visible_ = true;});
	
	auto back_button = button_box->AddWidget<Aegis::Button>("Exit");  
	back_button->SetSize({ 150, 50 });
	StylizeButton(*back_button, 3, 16);
	back_button->ConnectSignal("pressed", [&](){manager_->ReplaceScene<LevelSelectScene>(game_data, true);});
}

void LevelEditorScene::OnEvent(Aegis::Event& event)
{
	if (popup_->visible_ || controls_dialog_->visible_) return;

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

	if (!level_editor_->TilesAreValid()){
		return Error::InvalidTiles;
	}

	return Error::None;
}

void LevelEditorScene::PreviewLevel()
{
	Error error = IsLevelValid();
	if (error == Error::None){
		manager_->PushScene<GameplayScene>(tile_map_, game_data_);
	} else {
		popup_->Show(error);
	}
}

void LevelEditorScene::SaveLevel()
{
	Error error = IsLevelValid();
	if (error == Error::None){
		level_editor_->Save(level_num_);
		std::array<int, 2> star_times = {two_star_spinbox_->GetValue(), three_star_spinbox_->GetValue()}; 
		//if new level
		if (level_num_ == -1){
			++game_data_.num_custom_levels_;
			game_data_.custom_star_thresholds_.push_back(star_times);
			game_data_.custom_record_times_.push_back(-1);
			level_num_ = game_data_.num_custom_levels_;
		} else {
			game_data_.custom_star_thresholds_[level_num_ - 1] = star_times;
		}
		popup_->Show("Save Successful.");
	} else {
		popup_->Show(error);
	}
}

