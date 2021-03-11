#include "PauseMenu.h"
#include "Scenes/GameplayScene.h"
#include "Scenes/OptionsScene.h"
#include "Utilities.h"

PauseMenu::PauseMenu(Aegis::AABB rect, GameplayScene& scene)
{
	bg_texture_ = Aegis::Texture::Create("assets/textures/pause_menu_bg.png");

	rect_ = rect;
	Aegis::CenterAABB(rect_, Aegis::Application::GetWindow().GetViewport());

	v_box_ = std::make_shared<Aegis::Container>(Aegis::AABB(rect_.pos.x, rect_.pos.y + 50, rect_.size.x, rect_.size.y - 50), Aegis::Container::Vertical, 10, Aegis::Alignment::VCenter | Aegis::Alignment::HCenter);

	resume_button_ = v_box_->AddWidget<Aegis::Button>(Aegis::AABB(0, 0, 200, 50), "Resume");
	retry_button_ = v_box_->AddWidget<Aegis::Button>(Aegis::AABB(0, 0, 200, 50), "Retry");
	options_button_ = v_box_->AddWidget<Aegis::Button>(Aegis::AABB(0, 0, 200, 50), "Options");
	quit_button_ = v_box_->AddWidget<Aegis::Button>(Aegis::AABB(0, 0, 200, 50), "Quit");

	StylizeButton(*resume_button_, 3, 32);
	StylizeButton(*retry_button_, 3, 32);
	StylizeButton(*options_button_, 3, 32);
	StylizeButton(*quit_button_, 3, 32);

	resume_button_->ConnectSignal("pressed", [&]() {scene.Resume(); });
	retry_button_->ConnectSignal("pressed", [&]() {scene.SetUpLevel(); scene.Resume(); });
	options_button_->ConnectSignal("pressed", [&]() {scene.manager_->PushScene<OptionsScene>(); });
	quit_button_->ConnectSignal("pressed", [&]() {scene.manager_->PopScene(); });

	visible_ = false;
}

void PauseMenu::Render() const
{
	if (!visible_) return;

	Aegis::DrawQuad(rect_.pos, *bg_texture_);
	v_box_->Render();
}

void PauseMenu::OnEvent(Aegis::Event& event)
{
	if (!visible_) return;
	v_box_->OnEvent(event);
}


