#include "LevelSelectScene.h"

#include "../Gameplay/GameplayScene.h"
#include "../LevelEditor/LevelEditorScene.h"
#include "LevelCard.h"
#include "../../Utilities.h"

#include <filesystem>

void DeleteCustomLevel(int level, GameData& game_data);

LevelSelectScene::LevelSelectScene(GameData& game_data, bool show_custom)
{
	bg_ = Aegis::TextureManager::Load("assets/textures/scene_bg.png");

	ui_layer_ = std::make_unique<Aegis::UILayer>();

	Aegis::AABB rect = {0,0, 760, 720};
	Aegis::CenterAABBHorizontally(rect, Aegis::Application::GetWindow().GetViewport());
	v_box_ = ui_layer_->AddWidget<Aegis::VContainer>();
	v_box_->SetPos(rect.pos);
	v_box_->SetSize(rect.size);
	v_box_->SetPadding(16);
	v_box_->SetAlignment(Aegis::Alignment::HCenter);

	auto title_font = Aegis::FontManager::Load("assets/fonts/roboto_bold.ttf", 64);
	std::string title_text = show_custom ? "Custom Levels" : "Level Select";
	Aegis::AABB label_rect = {{0,20}, title_font->GetStringPixelSize(title_text)};
	Aegis::CenterAABBHorizontally(label_rect, Aegis::Application::GetWindow().GetViewport());
	auto label = v_box_->AddWidget<Aegis::Label>(title_text, label_rect.pos);
	label->SetFont(title_font);

	CreateLevelCards(game_data, show_custom);

	auto button_box = v_box_->AddWidget<Aegis::HContainer>();
	button_box->SetSize({760, 50});
	button_box->SetPadding(16);
	button_box->SetAlignment(Aegis::Alignment::Center);

	auto play_button = button_box->AddWidget<Aegis::Button>("Play");
	play_button->SetSize({ 200, 50 });
	play_button->ConnectSignal("pressed", [&, show_custom]() { if (selected_level_ != -1) manager_->ReplaceScene<GameplayScene>(selected_level_, show_custom, game_data); });
	StylizeButton(*play_button, 3, 24);

	auto back_button = button_box->AddWidget<Aegis::Button>("Back");
	back_button->SetSize({ 200, 50 });
	back_button->ConnectSignal("pressed", [&]() { manager_->PopScene();});
	StylizeButton(*back_button, 3, 24);

	if (!show_custom){
		auto custom_levels_button = v_box_->AddWidget<Aegis::Button>("Custom Levels");
		custom_levels_button->SetSize({ 200, 50 });
		custom_levels_button->ConnectSignal("pressed", [&]() {manager_->PushScene<LevelSelectScene>(game_data, true);});
		StylizeButton(*custom_levels_button, 3, 24);
	} else {
		auto button_box = v_box_->AddWidget<Aegis::HContainer>();
		button_box->SetSize({760, 50});
		button_box->SetPadding(16);
		button_box->SetAlignment(Aegis::Alignment::Center);

		auto new_button = button_box->AddWidget<Aegis::Button>("New");
		new_button->SetSize({ 200, 50 });
		auto edit_button = button_box->AddWidget<Aegis::Button>("Edit");
		edit_button->SetSize({ 200, 50 });
		auto delete_button = button_box->AddWidget<Aegis::Button>("Delete");
		delete_button->SetSize({ 200, 50 });

		err_dialog_ = ui_layer_->AddWidget<PopUpDialog>();
		new_button->ConnectSignal("pressed", [&]() { 
				if (game_data.num_custom_levels_ < 12){
					manager_->ReplaceScene<LevelEditorScene>(game_data);
				} else {
					err_dialog_->Show(Error::TooManyCustomLevels);
				}});
		edit_button->ConnectSignal("pressed", [&]() {if (selected_level_ != -1) manager_->ReplaceScene<LevelEditorScene>(game_data, selected_level_); });
		delete_button->ConnectSignal("pressed", [&]() {
			DeleteCustomLevel(selected_level_, game_data); 
			manager_->ReplaceScene<LevelSelectScene>(game_data, true);
		});
		StylizeButton(*new_button, 3, 24);
		StylizeButton(*edit_button, 3, 24);
		StylizeButton(*delete_button, 3, 24);
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
	Aegis::DrawQuad({0,0}, *bg_);
}

void LevelSelectScene::OnEvent(Aegis::Event& event)
{
}

void LevelSelectScene::CreateLevelCards(GameData& game_data, bool custom)
{
	auto level_card_box = v_box_->AddWidget<Aegis::VContainer>();
	level_card_box->SetSize({760, 480});
	level_card_box->SetPadding(24);
	level_card_box->SetAlignment(Aegis::Alignment::HCenter);

	std::shared_ptr<Aegis::Container> level_row_box;
	int num_levels = custom ? game_data.num_custom_levels_ : game_data.num_levels_;
	for (int i = 0; i < num_levels; ++i){
		if (i % 4 == 0){
			level_row_box = level_card_box->AddWidget<Aegis::HContainer>();
			level_row_box->SetSize({760, 128});
			level_row_box->SetPadding(24);
		}
		if (custom || i <= game_data.levels_completed_){
			std::shared_ptr<LevelCard> card;
			//custom levels currently do not have record times/stars
			if (custom){
				card = level_row_box->AddWidget<LevelCard>(i+1, std::array<int, 2>{}, -1);
			} else {
				card = level_row_box->AddWidget<LevelCard>(i+1, game_data.star_thresholds_[i], game_data.record_times_[i]);
			}
			card->ConnectSignal("double pressed", [&, level = i+1](){manager_->ReplaceScene<GameplayScene>(level, custom, game_data);});
			card->ConnectSignal("pressed", [&selected = selected_level_, level = i + 1](){ selected = level;}); 
		} else{
			level_row_box->AddWidget<LockedLevelCard>(i+1);
		}
	}
}

void DeleteCustomLevel(int level, GameData& game_data)
{
	if (level == -1 || level > game_data.num_custom_levels_) {
		return;
	}
	game_data.custom_star_thresholds_.erase(game_data.custom_star_thresholds_.begin() + level-1);
	game_data.custom_record_times_.erase(game_data.custom_record_times_.begin() + level-1);
	--game_data.num_custom_levels_; 

	std::string prefix = "assets/levels/custom_level_";

	std::filesystem::remove(prefix + std::to_string(level) + ".txt");

	++level;
	while (std::filesystem::exists(prefix + std::to_string(level) + ".txt")){
		std::filesystem::rename(prefix + std::to_string(level) + ".txt", prefix + std::to_string(level-1) + ".txt");
		++level;
	}
}

