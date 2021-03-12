#include "PauseMenu.h"
#include "Scenes/GameplayScene.h"
#include "Scenes/OptionsScene.h"
#include "Utilities.h"

PauseMenu::PauseMenu(GameplayScene& scene)
{
	bg_texture_ = Aegis::Texture::Create("assets/textures/pause_menu_bg.png");

	rect_ = { 0, 0, 240, 425 };
	Aegis::CenterAABB(rect_, Aegis::Application::GetWindow().GetViewport());

	v_box_ = std::make_shared<Aegis::Container>(Aegis::AABB(rect_.pos.x, rect_.pos.y + 50, rect_.size.x, rect_.size.y - 50), Aegis::Container::Vertical, 10, Aegis::Alignment::VCenter | Aegis::Alignment::HCenter);
	auto resume_button = v_box_->AddWidget<Aegis::Button>(Aegis::AABB(0, 0, 200, 50), "Resume");
	auto retry_button = v_box_->AddWidget<Aegis::Button>(Aegis::AABB(0, 0, 200, 50), "Retry");
	auto info_button = v_box_->AddWidget<Aegis::Button>(Aegis::AABB(0, 0, 200, 50), "Info");
	auto options_button = v_box_->AddWidget<Aegis::Button>(Aegis::AABB(0, 0, 200, 50), "Options");
	auto quit_button = v_box_->AddWidget<Aegis::Button>(Aegis::AABB(0, 0, 200, 50), "Quit");

	StylizeButton(*resume_button, 3, 32);
	StylizeButton(*retry_button, 3, 32);
	StylizeButton(*info_button, 3, 32);
	StylizeButton(*options_button, 3, 32);
	StylizeButton(*quit_button, 3, 32);

	resume_button->ConnectSignal("pressed", [&]() {scene.Resume(); });
	retry_button->ConnectSignal("pressed", [&]() {scene.SetUpLevel(); scene.Resume(); });
	info_button->ConnectSignal("pressed", [&]() {scene.info_dialog_->visible_ = true; });
	options_button->ConnectSignal("pressed", [&]() {scene.manager_->PushScene<OptionsScene>(); });
	quit_button->ConnectSignal("pressed", [&]() {scene.manager_->PopScene(); });

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


