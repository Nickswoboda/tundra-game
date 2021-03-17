#include "LevelCard.h"

#include "Utilities.h"

LevelCard::LevelCard(int level, std::array<double, 2> star_thresholds, double fastest_time)
	:Aegis::Widget({0,0, 160, 128})
{
	AddSignal("pressed");
	AddSignal("double pressed");
	v_box_ = std::make_shared<Aegis::Container>(rect_, Aegis::Container::Vertical, 4, Aegis::Alignment::HCenter);
	auto label = v_box_->AddWidget<Aegis::Label>(std::to_string(level), Aegis::Vec2());
	label->SetFont(Aegis::FontManager::Load("assets/fonts/roboto_bold.ttf", 24));
	auto star_container = v_box_->AddWidget<Aegis::Container>(Aegis::AABB(0,0, 100, 50), Aegis::Container::Horizontal, 1, Aegis::Alignment::VCenter | Aegis::Alignment::HCenter);
	auto sprite_sheet = Aegis::TextureManager::Load("assets/textures/tile_map.png");

	for (int i = 0; i < 3; ++i){
		auto star = star_container->AddWidget<Aegis::SpriteWidget>(Aegis::Vec2(), sprite_sheet, Aegis::AABB(0, 128, 32, 32));
	}
	
	auto font = Aegis::FontManager::Load("assets/fonts/roboto_regular.ttf", 14);

	auto label1 = v_box_->AddWidget<Aegis::Label>("Record: " + FormatTime(fastest_time), Aegis::Vec2());
	auto label2 = v_box_->AddWidget<Aegis::Label>("Next Star: --:--:--", Aegis::Vec2());

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
