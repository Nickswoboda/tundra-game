#include "ScoreCard.h"

ScoreCard::ScoreCard(const std::string& label, const std::array<double, 2>& star_thresholds)
	:Aegis::Dialog(label, {0,0, 400, 400}), star_thresholds_(star_thresholds)
{
	Aegis::CenterAABB(rect_, Aegis::Application::GetWindow().GetViewport());

	Aegis::Vec2 score_pos = {rect_.pos.x, rect_.pos.y + (rect_.size.y / 3)};
	Aegis::Vec2 score_size = rect_.size / Aegis::Vec2(1, 3);
	score_container_ = std::make_unique<Aegis::Container>(Aegis::AABB(score_pos, score_size), Aegis::Container::Vertical, 5, Aegis::Alignment::VCenter | Aegis::Alignment::HCenter); 
	time_label_ = score_container_->AddWidget<Aegis::Label>("", Aegis::Vec2(0,0));
	auto star_container = score_container_->AddWidget<Aegis::Container>(Aegis::AABB(0,0, 200, 50), Aegis::Container::Horizontal, 5, Aegis::Alignment::VCenter | Aegis::Alignment::HCenter);

	auto sprite_sheet = Aegis::TextureManager::Load("assets/textures/tile_map.png");
	for (int i = 0; i < 3; ++i){
		star_sprites_[i] = star_container->AddWidget<Aegis::SpriteWidget>(Aegis::Vec2(), sprite_sheet, Aegis::AABB(0, 128, 32, 32));
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

int ScoreCard::GetNumStarsEarned(double time) const 
{
	if (time <= star_thresholds_[0]){
		return 3;
	} else if (time <= star_thresholds_[1]){
		return 2;
	} else {
		return 1;
	}
}

std::string FormatTime(double time_in_seconds)
{
	int num_mins = time_in_seconds / 60;
	time_in_seconds -= num_mins * 60;

	int num_secs = time_in_seconds;
	time_in_seconds -= num_secs;

	int num_cs = time_in_seconds * 100;

	auto mins = num_mins >= 10 ? std::to_string(num_mins) : "0" + std::to_string(num_mins);
	auto secs = num_secs >= 10 ? std::to_string(num_secs) : "0" + std::to_string(num_secs);
	auto cs = num_cs >= 10 ? std::to_string(num_cs) : "0" + std::to_string(num_cs);

	return mins + ":" + secs + ":" + cs;
}

void ScoreCard::Show(double time)
{
	int num_stars = GetNumStarsEarned(time);
	for (int i = 0; i < 3; ++i){
		if (i < num_stars){
			star_sprites_[i]->sprite_.SetSubTextureRect({0, 128, 32, 32});
		} else {
			star_sprites_[i]->sprite_.SetSubTextureRect({32, 128, 32, 32});
		}
	}

	time_label_->SetText(FormatTime(time));
	visible_ = true;
}

