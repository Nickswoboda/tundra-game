#include "LevelSelectScene.h"

#include "GameplayScene.h"
#include "LevelEditorScene.h"
#include "../LevelCard.h"
#include "../Utilities.h"

#include <filesystem>


LevelSelectScene::LevelSelectScene(GameData& game_data)
{
	bg_ = Aegis::TextureManager::Load("assets/textures/scene_bg.png");

	ui_layer_ = std::make_unique<Aegis::UILayer>();

	Aegis::AABB rect = {0,0, 760, 720};
	Aegis::CenterAABBHorizontally(rect, Aegis::Application::GetWindow().GetViewport());
	v_box_ = ui_layer_->AddWidget<Aegis::Container>(rect, Aegis::Container::Vertical, 16, Aegis::Alignment::HCenter );

	auto title_font = Aegis::FontManager::Load("assets/fonts/roboto_bold.ttf", 64);
	Aegis::AABB label_rect = {{0,20}, title_font->GetStringPixelSize("Level Select")};
	Aegis::CenterAABBHorizontally(label_rect, Aegis::Application::GetWindow().GetViewport());
	auto label = v_box_->AddWidget<Aegis::Label>("Level Select", label_rect.pos);
	label->SetFont(title_font);

	auto level_card_box = v_box_->AddWidget<Aegis::Container>(Aegis::AABB(0,0, 760, 480), Aegis::Container::Vertical, 24, Aegis::Alignment::HCenter );
	std::shared_ptr<Aegis::Container> level_row_box;
	auto locked_texture = Aegis::TextureManager::Load("assets/textures/tile_map.png");
	for (int i = 0; i < game_data.num_levels_; ++i){
		if (i % 4 == 0){
			level_row_box = level_card_box->AddWidget<Aegis::Container>(Aegis::AABB(0,0, 760, 128), Aegis::Container::Horizontal, 24);
		}
		if (i <= game_data.levels_completed_){
			auto card = level_row_box->AddWidget<LevelCard>(i+1, game_data.star_thresholds_[i], game_data.record_times_[i]);
			card->ConnectSignal("double pressed", [&, level = i+1](){manager_->ReplaceScene<GameplayScene>(level, game_data);});
			card->ConnectSignal("pressed", [&selected = selected_level_, level = i + 1](){ selected = level;}); 
		} else{
			level_row_box->AddWidget<LockedLevelCard>(i+1);
		}
	}

	auto button_box = v_box_->AddWidget<Aegis::Container>(Aegis::AABB(0,0, 760, 50), Aegis::Container::Horizontal, 16, Aegis::Alignment::Center);
	auto play_button = button_box->AddWidget<Aegis::Button>(Aegis::AABB( 0, 0, 200, 50 ), "Play");
	auto back_button = button_box->AddWidget<Aegis::Button>(Aegis::AABB( 0, 0, 200, 50 ), "Back");
	auto custom_levels_button = v_box_->AddWidget<Aegis::Button>(Aegis::AABB( 0, 0, 200, 50 ), "Custom Levels");

	play_button->ConnectSignal("pressed", [&]() { if (selected_level_ != -1) manager_->ReplaceScene<GameplayScene>(selected_level_, game_data); });
	back_button->ConnectSignal("pressed", [&]() { manager_->PopScene();});
	custom_levels_button->ConnectSignal("pressed", [&]() {manager_->PushScene<LevelEditorScene>(game_data);});

	StylizeButton(*play_button, 3, 24);
	StylizeButton(*back_button, 3, 24);
	StylizeButton(*custom_levels_button, 3, 24);
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

	Aegis::DrawQuad({0,0}, *bg_);
}

void LevelSelectScene::OnEvent(Aegis::Event& event)
{
}
