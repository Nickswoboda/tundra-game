#include "LevelCard.h"

#include "../../Utilities.h"

int GetNumStarsEarned(const std::array<double, 2>& star_thresholds, const double time)
{
	if (time < 0) return 0;

	if (time <= star_thresholds[0]){
		return 3;
	} else if (time <= star_thresholds[1]){
		return 2;
	} else {
		return 1;
	}
}

LevelCard::LevelCard(int level, const std::array<double, 2>& star_thresholds, const double fastest_time)
	:Aegis::Widget({0,0, 160, 128})
{
	AddSignal("pressed");
	AddSignal("double pressed");
	v_box_ = std::make_shared<Aegis::Container>(rect_, Aegis::Container::Vertical, 4, Aegis::Alignment::Center);
	auto label = v_box_->AddWidget<Aegis::Label>(std::to_string(level), Aegis::Vec2());
	label->SetFont(Aegis::FontManager::Load("assets/fonts/roboto_bold.ttf", 24));
	auto star_container = v_box_->AddWidget<Aegis::Container>(Aegis::AABB(0,0, 100, 50), Aegis::Container::Horizontal, 1, Aegis::Alignment::VCenter | Aegis::Alignment::HCenter);
	auto sprite_sheet = Aegis::TextureManager::Load("assets/textures/tile_map.png");

	int num_stars_earned = GetNumStarsEarned(star_thresholds, fastest_time);
	for (int i = 0; i < 3; ++i){
		auto subtexture_rect = i < num_stars_earned ? Aegis::AABB(0, 128, 32, 32) : Aegis::AABB(32, 128, 32, 32);
		auto star = star_container->AddWidget<Aegis::SpriteWidget>(Aegis::Vec2(), sprite_sheet, subtexture_rect);
	}
	
	auto font = Aegis::FontManager::Load("assets/fonts/roboto_regular.ttf", 14);

	auto label1 = v_box_->AddWidget<Aegis::Label>("Record: " + FormatTime(fastest_time), Aegis::Vec2());

	double next_star_time = num_stars_earned == 3 || num_stars_earned == 0 ? -1 : star_thresholds[2 - num_stars_earned];
	auto label2 = v_box_->AddWidget<Aegis::Label>("Next Star: " + FormatTime(next_star_time), Aegis::Vec2());

	label1->SetFont(font);
	label2->SetFont(font);

	v_box_->UpdateWidgets();
}

void LevelCard::Render() const
{
	DrawFrameBorder(rect_);
	DrawQuad(rect_.pos, rect_.size, k_bg_color); 
	if (selected_){
		DrawQuad(rect_.pos, rect_.size, {0.9f, 0.9f, 0.9f, 0.2f}); 
	}
	v_box_->Render();
}

void LevelCard::OnEvent(Aegis::Event& event)
{
	auto click = dynamic_cast<Aegis::MouseClickEvent*>(&event);
	if (click) {
		if (PointInAABB(Aegis::Application::GetWindow().GetMousePos(), rect_)){
			if (click->action_ == AE_BUTTON_PRESS){
				Emit("pressed");
				selected_ = true;
			} else if (click->action_ == AE_MOUSE_DOUBLE_PRESS){
				Emit("double pressed");
				event.handled_ = true;
			}
		} else {
			selected_ = false;
		}
	}
}

void LevelCard::SetPos(Aegis::Vec2 pos)
{
	rect_.pos = pos;
	v_box_->SetPos(pos);
}

LockedLevelCard::LockedLevelCard(int level)
	: Aegis::Widget({0,0,160, 128})
{
	auto text = std::to_string(level);
	auto font = Aegis::FontManager::Load("assets/fonts/roboto_regular.ttf", 64);
	Aegis::Vec2 text_size = font->GetStringPixelSize(text);

	auto rect = Aegis::AABB(Aegis::Vec2{0,0}, text_size);
	Aegis::CenterAABB(rect, rect_);
	level_label_ = std::make_shared<Aegis::Label>(text, rect.pos, Aegis::Vec4(0.4f, 0.4f, 0.4f, 1.0f));
	level_label_->SetFont(font);
}

void LockedLevelCard::Render() const
{
	DrawFrameBorder(rect_);
	DrawQuad(rect_.pos, rect_.size, {0.72f, 0.71f, 0.74f, 1.0f}); 
	level_label_->Render();
}

void LockedLevelCard::OnEvent(Aegis::Event& event)
{
}

void LockedLevelCard::SetPos(Aegis::Vec2 pos)
{
	Aegis::Vec2 offset = pos - rect_.pos;
	rect_.pos = pos;
	level_label_->SetPos(level_label_->GetRect().pos + offset);
}
