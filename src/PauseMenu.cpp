#include "PauseMenu.h"

PauseMenu::PauseMenu(Aegis::AABB rect)
{
	rect_ = rect;
	Aegis::AABB window_size{ 0, 0, Aegis::Application::GetWindow().GetSize().x, Aegis::Application::GetWindow().GetSize().y };
	Aegis::CenterAABBHorizontally(rect_, window_size);
	Aegis::CenterAABBVertically(rect_, window_size);
	v_box_ = std::make_shared<Aegis::Container>(rect_, Aegis::Container::Vertical, 10, Aegis::Alignment::VCenter | Aegis::Alignment::HCenter);

	continue_button_ = std::make_shared<Aegis::Button>(Aegis::AABB(0,0, rect.size.x / 2, rect.size.y / 6), "Continue");
	retry_button_ = std::make_shared<Aegis::Button>(Aegis::AABB(0,0, rect.size.x / 2, rect.size.y / 6), "Retry");
	options_button_ = std::make_shared<Aegis::Button>(Aegis::AABB(0,0, rect.size.x / 2, rect.size.y / 6), "Options");
	quit_button_ = std::make_shared<Aegis::Button>(Aegis::AABB(0,0, rect.size.x / 2, rect.size.y / 6), "Quit");

	v_box_->AddWidget(continue_button_);
	v_box_->AddWidget(retry_button_);
	v_box_->AddWidget(options_button_);
	v_box_->AddWidget(quit_button_);

	visible_ = false;
}

void PauseMenu::Render() const
{
	if (!visible_) return;

	v_box_->Render();
	continue_button_->Render();
	retry_button_->Render();
	options_button_->Render();
	quit_button_->Render();
}

void PauseMenu::OnEvent(Aegis::Event& event)
{
	continue_button_->OnEvent(event);
	retry_button_->OnEvent(event);
	options_button_->OnEvent(event);
	quit_button_->OnEvent(event);
}


