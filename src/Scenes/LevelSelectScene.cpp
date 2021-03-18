#include "LevelSelectScene.h"

#include "GameplayScene.h"
#include "LevelEditorScene.h"
#include "../LevelCard.h"

#include <filesystem>
LevelSelectScene::LevelSelectScene(GameData& game_data)
{
	title_font_ = Aegis::FontManager::Load("assets/fonts/worksans_regular.ttf", 64);
	button_font_ = Aegis::FontManager::Load("assets/fonts/worksans_regular.ttf", 32);

	ui_layer_ = std::make_unique<Aegis::UILayer>();
	ui_layer_->SetFont(button_font_);

	auto back_button = ui_layer_->AddWidget<Aegis::Button>(Aegis::AABB( 560, 650, 120, 30 ), "Back");
	auto select_button = ui_layer_->AddWidget<Aegis::Button>(Aegis::AABB( 500, 550, 120, 30 ), "Select");
	auto edit_button = ui_layer_->AddWidget<Aegis::Button>(Aegis::AABB( 630, 550, 120, 30 ), "Edit");
	auto create_level_button = ui_layer_->AddWidget<Aegis::Button>(Aegis::AABB( 520, 600, 220, 30 ), "New Level");

	back_button->ConnectSignal("pressed", [&]() { manager_->PopScene();});
	select_button->ConnectSignal("pressed", [&]() { if (selected_level_ != -1) manager_->PushScene<GameplayScene>(selected_level_, game_data); });
	edit_button->ConnectSignal("pressed", [&]() { if (selected_level_ != -1) manager_->PushScene<LevelEditorScene>(game_data, selected_level_);});
	create_level_button->ConnectSignal("pressed", [&]() {manager_->PushScene<LevelEditorScene>(game_data);});

	Aegis::AABB rect = {0,0, 800, 600};
	Aegis::CenterAABB(rect, Aegis::Application::GetWindow().GetViewport());
	auto level_card_box = ui_layer_->AddWidget<Aegis::Container>(rect, Aegis::Container::Vertical, 16, Aegis::Alignment::HCenter );
	std::shared_ptr<Aegis::Container> level_row_box;
	for (int i = 0; i < game_data.num_levels_; ++i){
		if (i % 4 == 0){
			level_row_box = level_card_box->AddWidget<Aegis::Container>(Aegis::AABB(0,0, 800, 128), Aegis::Container::Horizontal, 16);
		}
		auto card = level_row_box->AddWidget<LevelCard>(i+1, game_data.star_thresholds_[i], game_data.record_times_[i]);
		card->ConnectSignal("double pressed", [&, level = i+1](){manager_->ReplaceScene<GameplayScene>(level, game_data);});
		card->ConnectSignal("pressed", [&selected = selected_level_, level = i + 1](){ selected = level;}); 
	}
}

LevelSelectScene::~LevelSelectScene()
{
}
void LevelSelectScene::Update()
{

}

void LevelSelectScene::Render(float delta_time)
{
	Aegis::Renderer2D::SetProjection(camera_.view_projection_matrix_);

	Aegis::Renderer2D::SetFont(title_font_);
	Aegis::DrawText("Level Select", { 490, 20 });
}

void LevelSelectScene::OnEvent(Aegis::Event& event)
{
}
