#include "OptionsScene.h"

OptionsScene::OptionsScene()
{
	title_font_ = Aegis::FontManager::Instance().Load("assets/fonts/WorkSans-Regular.ttf", 64);
	button_font_ = Aegis::FontManager::Instance().Load("assets/fonts/WorkSans-Regular.ttf", 32);

	screen_mode_dropdown_ = new Aegis::Dropdown("Screen Mode", { 580, 150, 200, 32 });
	screen_mode_dropdown_->AddItem("Windowed", []() {Aegis::Application::GetWindow().SetScreenMode(Aegis::ScreenMode::Windowed); });
	screen_mode_dropdown_->AddItem("Fullscreen", []() {Aegis::Application::GetWindow().SetScreenMode(Aegis::ScreenMode::Fullscreen); });
	screen_mode_dropdown_->AddItem("Fullscreen Windowed", []() {Aegis::Application::GetWindow().SetScreenMode(Aegis::ScreenMode::FullscreenWindow); });

	resolution_dropdown_ = new Aegis::Dropdown("Resolution", { 580, 260, 200, 32 });
	resolution_dropdown_->AddItem("1280x720", [&]() {SetResolution(1280, 720); });
	resolution_dropdown_->AddItem("1600x900", [&]() {SetResolution(1600, 900); });
	resolution_dropdown_->AddItem("1920x1080", [&]() {SetResolution(1920, 1080); });

	vsync_ = Aegis::Application::GetWindow().IsVsync();
	toggle_vsync_button_ = new Aegis::Button({ 780, 400, 200, 32 }, vsync_ ? "On" : "Off", button_font_);

	back_button_ = new Aegis::Button({ 580, 600, 200, 32 }, "Back", button_font_);
}

OptionsScene::~OptionsScene()
{
}

void OptionsScene::Update()
{
}

void OptionsScene::Render(float delta_time)
{
	Aegis::Renderer2D::BeginScene(camera_.view_projection_matrix_);
	Aegis::RendererClear();

	Aegis::Renderer2D::SetFont(title_font_);
	Aegis::DrawText("Options", { 500, 20 }, { 1.0f, 1.0f, 1.0f, 1.0f });

	Aegis::Renderer2D::SetFont(button_font_);
	screen_mode_dropdown_->Render();

	resolution_dropdown_->Render();

	Aegis::DrawText("Vsync: ", { 580, toggle_vsync_button_->rect_.pos.y }, { 1.0f, 1.0f, 1.0f, 1.0f });
	toggle_vsync_button_->Render();
	back_button_->Render();

	Aegis::Renderer2D::EndScene();
}

void OptionsScene::OnEvent(Aegis::Event& event)
{
	auto click_event = dynamic_cast<Aegis::MouseClickEvent*>(&event);
	if (click_event) {
		if (toggle_vsync_button_->IsPressed(click_event->action_)) {
			if (vsync_) {
				Aegis::Application::GetWindow().SetVsync(false);
				toggle_vsync_button_->text_ = "Off";
				vsync_ = false;
			}
			else{
				Aegis::Application::GetWindow().SetVsync(true);
				toggle_vsync_button_->text_ = "On";
				vsync_ = true;
			}
		}
		if (screen_mode_dropdown_->IsPressed(click_event->action_)) {

		}
		if (resolution_dropdown_->IsPressed(click_event->action_)) {

		}
		if (back_button_->IsPressed(click_event->action_)) {
			manager_->PopScene();
		}
	}
}

void OptionsScene::SetResolution(int x, int y)
{
	Aegis::Application::GetWindow().SetResolution(x, y);
	Aegis::Application::GetWindow().SetSize(x, y);
	Aegis::Application::GetWindow().CenterOnScreen();
	manager_->UpdateAllCameraProjections(0, x, y, 0);

}
