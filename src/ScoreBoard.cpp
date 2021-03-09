#include "ScoreBoard.h"

ScoreBoard::ScoreBoard(int num_lives, int num_pellets)
{
	auto sprite_sheet = Aegis::TextureManager::Load("assets/textures/tile_map.png");
	bg_ = std::make_unique<Aegis::SpriteWidget>(Aegis::Vec2(20, 16), Aegis::TextureManager::Load("assets/textures/score_frame.png"));
	v_box_ = std::make_unique<Aegis::Container>(Aegis::AABB(20, 16, 96, 128), Aegis::Container::Vertical, 8, Aegis::Alignment::Center);
	auto heart_container = v_box_->AddWidget<Aegis::Container>(Aegis::AABB( 0, 0, 96, 16 ), Aegis::Container::Horizontal, 4, Aegis::Alignment::VCenter | Aegis::Alignment::HCenter);
	for (int i = 0; i < num_lives; ++i) {
		heart_widgets_[i] = heart_container->AddWidget<Aegis::SpriteWidget>(Aegis::Vec2(), sprite_sheet, Aegis::AABB(128, 112, 16, 16));
	}
	auto fish_container = v_box_->AddWidget<Aegis::Container>(Aegis::AABB(0, 0, 96, 55), Aegis::Container::Vertical, 2, Aegis::Alignment::Center);
	fish_container->AddWidget<Aegis::SpriteWidget>(Aegis::Vec2(), sprite_sheet, Aegis::AABB(96, 96, 32, 32));
	pellet_count_label_ = fish_container->AddWidget<Aegis::Label>("0/" + std::to_string(num_pellets), Aegis::Vec2(), Aegis::Vec4(0, 0, 0, 1));
	time_label_ = v_box_->AddWidget<Aegis::Label>("00:00:00", Aegis::Vec2(), Aegis::Vec4(0,0,0,1));
}

void ScoreBoard::Render() const
{
	bg_->Render();
	v_box_->Render();
}

void ScoreBoard::OnEvent(Aegis::Event& event)
{
	v_box_->OnEvent(event);
}

void ScoreBoard::SetTimeLabel(const std::string& label)
{
	time_label_->SetText(label);
}

void ScoreBoard::SetNumLives(int num_lives)
{
	for (int i = 0; i < 3; ++i){
		if (i < num_lives){
			heart_widgets_[i]->sprite_.SetSubTextureRect({128, 112, 16, 16});
		} else {
			heart_widgets_[i]->sprite_.SetSubTextureRect({128, 96, 16, 16});
		}
	}
}

void ScoreBoard::SetPelletCount(int current, int total)
{
	pellet_count_label_->SetText(std::to_string(current) + "/" + std::to_string(total));
}
