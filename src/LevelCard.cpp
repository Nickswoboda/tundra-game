#include "LevelCard.h"

#include "Utilities.h"

LevelCard::LevelCard(int level)
	:Aegis::Widget({0,0, 100, 100})
{
	v_box_ = std::make_shared<Aegis::Container>(rect_, Aegis::Container::Vertical, 4, Aegis::Alignment::Center);
	auto label = v_box_->AddWidget<Aegis::Label>(std::to_string(level), Aegis::Vec2());
	label->SetFont(Aegis::FontManager::Load("assets/fonts/roboto_bold.ttf", 24));
}

void LevelCard::Render() const
{
	DrawFrameBorder(rect_);
	DrawQuad(rect_.pos, rect_.size, k_bg_color); 
	v_box_->Render();
}

void LevelCard::OnEvent(Aegis::Event& event)
{
}

void LevelCard::SetPos(Aegis::Vec2 pos)
{
	rect_.pos = pos;
	v_box_->SetPos(pos);
}
