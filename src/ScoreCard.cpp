#include "ScoreCard.h"

ScoreCard::ScoreCard(const std::string& label, const Aegis::AABB& rect)
	:Aegis::Dialog(label, rect)
{
	int x_pos = rect_.pos.x;
	int y_pos = rect_.pos.y + (rect_.size.y / 3);
	score_container_ = std::make_unique<Aegis::Container>(Aegis::AABB(x_pos,y_pos,rect_.size.x, rect_.size.y / 3), Aegis::Container::Vertical, 5, Aegis::Alignment::VCenter | Aegis::Alignment::HCenter); 
	time_label_ = score_container_->AddWidget<Aegis::Label>("", Aegis::Vec2(0,0));
	auto star_container = score_container_->AddWidget<Aegis::Container>(Aegis::AABB(0,0, 100, 50), Aegis::Container::Horizontal, 5, Aegis::Alignment::VCenter | Aegis::Alignment::HCenter);

	auto sprite_sheet = Aegis::TextureManager::Load("assets/textures/tile_map.png");
	for (int i = 0; i < 3; ++i){
		star_sprites_[i] = star_container->AddWidget<Aegis::SpriteWidget>(Aegis::Vec2(), sprite_sheet, Aegis::AABB(128, 112, 16, 16));
	}
}

void ScoreCard::Render() const 
{
	if (visible_){
		Aegis::DrawQuad(rect_.pos, rect_.size, { 0.0f, 0.0f, 1.0f, 1.0f });
		button_container_->Render();
		score_container_->Render();
	}
}

void ScoreCard::OnEvent(Aegis::Event& event)
{
	if (visible_){
		button_container_->OnEvent(event);
		score_container_->OnEvent(event);
	}
}

void ScoreCard::Show(double time, int stars)
{
	for (int i = 0; i < 3; ++i){
		if (i < stars){
			star_sprites_[i]->sprite_.SetSubTextureRect({128, 112, 16, 16});
		} else {
			star_sprites_[i]->sprite_.SetSubTextureRect({128, 96, 16, 16});
		}
	}
	time_label_->SetText(std::to_string(time));
	visible_ = true;
}


