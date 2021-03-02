#include "LevelSelectScene.h"

#include "GameplayScene.h"
#include "LevelEditorScene.h"

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

	int level = 1;
	int x_pos = 500;
	int y_pos = 200;
	while (std::filesystem::exists("assets/levels/level_" + std::to_string(level) + ".txt")){
		auto button = ui_layer_->AddWidget<Aegis::Button>(Aegis::AABB(x_pos, y_pos, 64, 64), std::to_string(level));
		button->ConnectSignal("pressed", [&, level]() { selected_level_ = level;});
		button->ConnectSignal("double pressed", [&, level]() {manager_->PushScene<GameplayScene>(level, game_data);});
		level_buttons_.push_back(button);
		x_pos += 74;

		//maximum of 4 buttons per row
		if (level % 4 == 0){
			x_pos = 500;
			y_pos += 74;
		}

		++level;
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

	if (selected_level_ != -1) {
		auto pos = level_buttons_[selected_level_ - 1]->GetRect().pos;
		auto size = level_buttons_[selected_level_ - 1]->GetRect().size;
		Aegis::DrawQuad(pos, size, { 1.0f, 1.0f, 1.0f, 0.4 });
	}
}

void LevelSelectScene::OnEvent(Aegis::Event& event)
{
}
