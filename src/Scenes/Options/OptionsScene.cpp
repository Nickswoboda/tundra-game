#include "OptionsScene.h"

OptionsScene::OptionsScene()
{
	title_font_ = Aegis::FontManager::Load("assets/fonts/worksans_regular.ttf", 64);
	button_font_ = Aegis::FontManager::Load("assets/fonts/worksans_regular.ttf", 32);

	ui_layer_ = std::make_unique<Aegis::UILayer>();
	ui_layer_->SetFont(button_font_);

	auto screen_mode_dropdown_ = ui_layer_->AddWidget<Aegis::Dropdown>("Screen Mode", Aegis::AABB( 580, 150, 200, 32 ));
	screen_mode_dropdown_->AddItem("Fullscreen", []() {Aegis::Application::GetWindow().SetScreenMode(Aegis::ScreenMode::Fullscreen); });
	screen_mode_dropdown_->AddItem("Windowed", []() {Aegis::Application::GetWindow().SetScreenMode(Aegis::ScreenMode::Windowed); });
	screen_mode_dropdown_->AddItem("Fullscreen Windowed", []() {Aegis::Application::GetWindow().SetScreenMode(Aegis::ScreenMode::FullscreenWindow); });
	screen_mode_dropdown_->MoveToTop((int)Aegis::Application::GetWindow().GetScreenMode());

	auto resolution_dropdown_ = ui_layer_->AddWidget<Aegis::Dropdown>("Resolution", Aegis::AABB( 580, 210, 200, 32 ));
	resolution_dropdown_->AddItem("1280x720", [&]() {SetResolution(1280, 720); });
	resolution_dropdown_->AddItem("1600x900", [&]() {SetResolution(1600, 900); });
	resolution_dropdown_->AddItem("1920x1080", [&]() {SetResolution(1920, 1080); });

	//quick way of determinning which item is shown first in dropdown. Will be removed when I get to implementing a true UI system
	if (Aegis::Application::GetWindow().GetResolution().x == 1600){
		resolution_dropdown_->MoveToTop(1);
	}
	else if (Aegis::Application::GetWindow().GetResolution().x == 1920) {
		resolution_dropdown_->MoveToTop(2);
	}

	auto vsync_checkbox_ = ui_layer_->AddWidget<Aegis::Checkbox>("Vsync", Aegis::AABB( 780, 400, 200, 32 ));
	vsync_checkbox_->ConnectSignal("checked", [](){Aegis::Application::GetWindow().SetVsync(true);});
	vsync_checkbox_->ConnectSignal("unchecked", [](){Aegis::Application::GetWindow().SetVsync(false);});

	auto back_button_ = ui_layer_->AddWidget<Aegis::Button>(Aegis::AABB( 580, 600, 200, 32 ), "Back");
	back_button_->ConnectSignal("pressed", [&](){manager_->PopScene();});
}

OptionsScene::~OptionsScene()
{
}

void OptionsScene::Update()
{
}

void OptionsScene::Render(float delta_time)
{
	Aegis::Renderer2D::SetProjection(camera_.view_projection_matrix_);

	Aegis::Renderer2D::SetFont(title_font_);
	Aegis::DrawText("Options", { 500, 20 });
}

void OptionsScene::OnEvent(Aegis::Event& event)
{
}

void OptionsScene::SetResolution(int x, int y)
{
	Aegis::Application::GetWindow().SetResolution(x, y);
	//Aegis::Application::GetWindow().SetSize(x, y);
	Aegis::Application::GetWindow().CenterOnScreen();
	manager_->UpdateAllCameraProjections(0, x, y, 0);

}
