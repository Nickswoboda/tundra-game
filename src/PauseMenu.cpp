#include "PauseMenu.h"

PauseMenu::PauseMenu(Aegis::AABB rect)
{
	bg_texture_ = Aegis::Texture::Create("assets/textures/pause_menu_bg.png");
	rect_ = rect;
	Aegis::AABB window_size{ 0, 0, Aegis::Application::GetWindow().GetSize().x, Aegis::Application::GetWindow().GetSize().y };
	Aegis::CenterAABBHorizontally(rect_, window_size);
	Aegis::CenterAABBVertically(rect_, window_size);
	v_box_ = std::make_shared<Aegis::Container>(Aegis::AABB(rect_.pos.x, rect_.pos.y + 50, rect_.size.x, rect_.size.y - 50), Aegis::Container::Vertical, 10, Aegis::Alignment::VCenter | Aegis::Alignment::HCenter);

	continue_button_ = std::make_shared<Aegis::Button>(Aegis::AABB(0,0, 200, 50), "");
	continue_button_->SetStateTexture(Aegis::Button::Normal, Aegis::Texture::Create("assets/textures/resume_btn.png"));
	continue_button_->SetStateTexture(Aegis::Button::Hovered, Aegis::Texture::Create("assets/textures/resume_btn_hovered.png"));
	retry_button_ = std::make_shared<Aegis::Button>(Aegis::AABB(0, 0, 200, 50), "");
	retry_button_->SetStateTexture(Aegis::Button::Normal, Aegis::Texture::Create("assets/textures/restart_btn.png"));
	retry_button_->SetStateTexture(Aegis::Button::Hovered, Aegis::Texture::Create("assets/textures/restart_btn_hovered.png"));
	options_button_ = std::make_shared<Aegis::Button>(Aegis::AABB(0, 0, 200, 50), "");
	options_button_->SetStateTexture(Aegis::Button::Normal, Aegis::Texture::Create("assets/textures/options_btn.png"));
	options_button_->SetStateTexture(Aegis::Button::Hovered, Aegis::Texture::Create("assets/textures/options_btn_hovered.png")); 
	quit_button_ = std::make_shared<Aegis::Button>(Aegis::AABB(0, 0, 200, 50), "");
	quit_button_->SetStateTexture(Aegis::Button::Normal, Aegis::Texture::Create("assets/textures/quit_btn.png"));
	quit_button_->SetStateTexture(Aegis::Button::Hovered, Aegis::Texture::Create("assets/textures/quit_btn_hovered.png"));

	v_box_->AddWidget(continue_button_);
	v_box_->AddWidget(retry_button_);
	v_box_->AddWidget(options_button_);
	v_box_->AddWidget(quit_button_);

	visible_ = false;
}

void PauseMenu::Render() const
{
	if (!visible_) return;

	Aegis::DrawQuad(rect_.pos, *bg_texture_);
	v_box_->Render();
	continue_button_->Render();
	retry_button_->Render();
	options_button_->Render();
	quit_button_->Render();
}

void PauseMenu::OnEvent(Aegis::Event& event)
{
	if (!visible_) return;
	continue_button_->OnEvent(event);
	retry_button_->OnEvent(event);
	options_button_->OnEvent(event);
	quit_button_->OnEvent(event);
}


