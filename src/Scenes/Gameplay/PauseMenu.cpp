#include "PauseMenu.h"
#include "GameplayScene.h"
#include "../Options/OptionsScene.h"
#include "../../Utilities.h"

PauseMenu::PauseMenu(GameplayScene& scene)
{
	rect_ = { 0, 0, 240, 400 };
	Aegis::CenterAABB(rect_, Aegis::Application::GetWindow().GetViewport());

	v_box_.SetPos(rect_.pos);
	v_box_.SetSize(rect_.size);
	v_box_.SetPadding(10);
	v_box_.SetAlignment(Aegis::Alignment::Center);
	
	auto label = std::make_shared<Aegis::Label>("PAUSED", Aegis::Vec2());
	label->SetFont(Aegis::FontManager::Load("assets/fonts/roboto_bold.ttf", 40));
	v_box_.AddWidget(label);

	auto resume_button = v_box_.AddWidget<Aegis::Button>(Aegis::AABB(0, 0, 200, 50), "Resume");
	auto retry_button = v_box_.AddWidget<Aegis::Button>(Aegis::AABB(0, 0, 200, 50), "Retry");
	auto info_button = v_box_.AddWidget<Aegis::Button>(Aegis::AABB(0, 0, 200, 50), "Info");
	auto options_button = v_box_.AddWidget<Aegis::Button>(Aegis::AABB(0, 0, 200, 50), "Options");
	auto quit_button = v_box_.AddWidget<Aegis::Button>(Aegis::AABB(0, 0, 200, 50), "Quit");

	StylizeButton(*resume_button, 3, 32);
	StylizeButton(*retry_button, 3, 32);
	StylizeButton(*info_button, 3, 32);
	StylizeButton(*options_button, 3, 32);
	StylizeButton(*quit_button, 3, 32);

	resume_button->ConnectSignal("pressed", [&]() {scene.Resume(); });
	retry_button->ConnectSignal("pressed", [&]() {scene.SetUpLevel(); scene.Resume(); });
	info_button->ConnectSignal("pressed", [&]() {scene.info_dialog_->visible_ = true; });
	options_button->ConnectSignal("pressed", [&]() {scene.manager_->PushScene<OptionsScene>(scene.game_data_); });
	quit_button->ConnectSignal("pressed", [&]() {scene.manager_->PopScene(); });

	visible_ = false;
}

void PauseMenu::Render() const
{
	if (!visible_) return;

	DrawFrameBorder(rect_);
	Aegis::DrawQuad(rect_.pos, rect_.size, k_bg_color);
	v_box_.Render();
}

void PauseMenu::OnEvent(Aegis::Event& event)
{
	if (!visible_) return;
	v_box_.OnEvent(event);
}


