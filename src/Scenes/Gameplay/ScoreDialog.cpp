#include "ScoreDialog.h"

#include "../../Utilities.h"
#include "GameplayScene.h"
#include "../LevelSelect/LevelSelectScene.h"

ScoreDialog::ScoreDialog(GameplayScene& scene)
	:Aegis::Dialog("", {0,0, 350, 200})
{
	if (scene.custom_level_){
		star_thresholds_ = scene.game_data_.custom_star_thresholds_[scene.level_ - 1];
	}else {
		star_thresholds_ = scene.game_data_.star_thresholds_[scene.level_ - 1];
	}

	Aegis::AABB rect {0,0, 300, 200};
	Aegis::CenterAABB(rect, Aegis::Application::GetWindow().GetViewport());
	SetPos(rect.pos);

	Aegis::Vec2 score_pos = {rect_.pos.x, rect_.pos.y + (rect_.size.y / 3)};
	Aegis::Vec2 score_size = rect_.size / Aegis::Vec2(1, 2);
	score_container_ = std::make_unique<Aegis::Container>(Aegis::AABB(score_pos, score_size), Aegis::Container::Vertical, 5, Aegis::Alignment::VCenter | Aegis::Alignment::HCenter); 
	time_label_ = score_container_->AddWidget<Aegis::Label>("", Aegis::Vec2(0,0));
	auto star_container = score_container_->AddWidget<Aegis::Container>(Aegis::AABB(0,0, 200, 50), Aegis::Container::Horizontal, 5, Aegis::Alignment::VCenter | Aegis::Alignment::HCenter);
	auto sprite_sheet = Aegis::TextureManager::Load("assets/textures/tile_map.png");
	for (int i = 0; i < 3; ++i){
		star_sprites_[i] = star_container->AddWidget<Aegis::SpriteWidget>(Aegis::Vec2(), sprite_sheet, Aegis::AABB(0, 128, 32, 32));
	}
	bg_color_ =  k_bg_color;

	bool last_level = scene.level_ == scene.game_data_.num_levels_;
	if (last_level){
		text_->SetText("Congratulations! You beat the game!");
		AddButton("Level Select", [&]() {scene.manager_->ReplaceScene<LevelSelectScene>(scene.game_data_); });
		AddButton("Main Menu", [&]() {scene.manager_->PopScene(); });
	} else {
		text_->SetText("Congratulations! You won!");
		AddButton("Next Level", [&]() {scene.manager_->ReplaceScene<GameplayScene>(scene.level_ + 1, false, scene.game_data_); });
		AddButton("Replay Level", [&]() {scene.SetUpLevel(); });
		AddButton("Main Menu", [&]() {scene.manager_->PopScene(); });
	}

	text_->SetFont(Aegis::FontManager::Load("assets/fonts/roboto_regular.ttf", last_level ? 20 : 28));
	auto text_rect = Aegis::AABB({ 0, 0 }, text_->font_->GetStringPixelSize(text_->GetText()));
	Aegis::CenterAABBHorizontally(text_rect, rect_);
	text_->SetPos({text_rect.pos.x, rect_.pos.y + rect_.size.y / 6});

}

void ScoreDialog::Render() const 
{
	if (visible_){
		DrawFrameBorder(rect_);
		Dialog::Render();
		score_container_->Render();
	}
}

void ScoreDialog::OnEvent(Aegis::Event& event)
{
	if (visible_){
		button_container_->OnEvent(event);
		score_container_->OnEvent(event);
	}
}

int ScoreDialog::GetNumStarsEarned(double time) const
{
	if (time <= star_thresholds_[1]){
		return 3;
	} else if (time <= star_thresholds_[0]){
		return 2;
	} else {
		return 1;
	}
}

void ScoreDialog::Show(double time) 
{
	int num_stars = GetNumStarsEarned(time);
	for (int i = 0; i < 3; ++i){
		if (i < num_stars){
			star_sprites_[i]->SetSubTextureRect({0, 128, 32, 32});
		} else {
			star_sprites_[i]->SetSubTextureRect({32, 128, 32, 32});
		}
	}

	time_label_->SetText(FormatTime(time));
	visible_ = true;
}

void ScoreDialog::AddButton(const std::string& label, std::function<void()> callback)
{
	auto button = button_container_->AddWidget<Aegis::Button>(Aegis::AABB(0,0, 100, 40), label);
	button->ConnectSignal("pressed", [&visible = visible_, callback]{visible = false; callback();});
	StylizeButton(*button, 2, 16);
}
