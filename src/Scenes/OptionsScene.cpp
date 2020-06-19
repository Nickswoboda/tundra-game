#include "OptionsScene.h"

OptionsScene::OptionsScene()
{
	title_font_ = Aegis::FontManager::Instance().Load("assets/fonts/WorkSans-Regular.ttf", 64);
	button_font_ = Aegis::FontManager::Instance().Load("assets/fonts/WorkSans-Regular.ttf", 32);

	ui_layer_ = new Aegis::UILayer();
	screen_mode_dropdown_ = ui_layer_->AddDropdown("Screen Mode", { 580, 150, 200, 32 });
	screen_mode_dropdown_->AddItem("Fullscreen", []() {Aegis::Application::GetWindow().SetScreenMode(Aegis::ScreenMode::Fullscreen); });
	screen_mode_dropdown_->AddItem("Windowed", []() {Aegis::Application::GetWindow().SetScreenMode(Aegis::ScreenMode::Windowed); });
	screen_mode_dropdown_->AddItem("Fullscreen Windowed", []() {Aegis::Application::GetWindow().SetScreenMode(Aegis::ScreenMode::FullscreenWindow); });
	screen_mode_dropdown_->SetCurrentIndex((int)Aegis::Application::GetWindow().GetScreenMode());

	resolution_dropdown_ = ui_layer_->AddDropdown("Resolution", { 580, 210, 200, 32 });
	resolution_dropdown_->AddItem("1280x720", [&]() {SetResolution(1280, 720); });
	resolution_dropdown_->AddItem("1600x900", [&]() {SetResolution(1600, 900); });
	resolution_dropdown_->AddItem("1920x1080", [&]() {SetResolution(1920, 1080); });


	//quick way of determinning which item is shown first in dropdown. Will be removed when I get to implementing a true UI system
	if (Aegis::Application::GetWindow().GetResolution().x == 1600){
		resolution_dropdown_->SetCurrentIndex(1);
	}
	else if (Aegis::Application::GetWindow().GetResolution().x == 1920) {
		resolution_dropdown_->SetCurrentIndex(2);
	}

	vsync_ = Aegis::Application::GetWindow().IsVsync();
	toggle_vsync_button_ = ui_layer_->AddButton({ 780, 400, 200, 32 }, vsync_ ? "On" : "Off", button_font_, [&]() {
		if (vsync_) {
			Aegis::Application::GetWindow().SetVsync(false);
			toggle_vsync_button_->text_ = "Off";
			vsync_ = false;
		}
		else {
			Aegis::Application::GetWindow().SetVsync(true);
			toggle_vsync_button_->text_ = "On";
			vsync_ = true;
		}});

	back_button_ = ui_layer_->AddButton({ 580, 600, 200, 32 }, "Back", button_font_, [&]() {manager_->PopScene(); });


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
	Aegis::RendererClear();

	Aegis::Renderer2D::SetFont(title_font_);
	Aegis::DrawText("Options", { 500, 20 }, { 1.0f, 1.0f, 1.0f, 1.0f });

	Aegis::Renderer2D::SetFont(button_font_);

	Aegis::DrawText("Vsync: ", { 580, toggle_vsync_button_->rect_.pos.y }, { 1.0f, 1.0f, 1.0f, 1.0f });
}

void OptionsScene::OnEvent(Aegis::Event& event)
{
}

void OptionsScene::SetResolution(int x, int y)
{
	Aegis::Application::GetWindow().SetResolution(x, y);
	Aegis::Application::GetWindow().SetSize(x, y);
	Aegis::Application::GetWindow().CenterOnScreen();
	manager_->UpdateAllCameraProjections(0, x, y, 0);

}
