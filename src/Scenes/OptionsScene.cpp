#include "OptionsScene.h"

#include "../FontManager.h"

OptionsScene::OptionsScene()
{
	title_font_ = FontManager::Instance().Load("assets/fonts/WorkSans-Regular.ttf", 64);
	button_font_ = FontManager::Instance().Load("assets/fonts/WorkSans-Regular.ttf", 32);

	windowed_button_ = new Aegis::Button({ 580, 150, 200, 32 }, "Windowed Mode", button_font_);
	fullscreen_button_ = new Aegis::Button({ 580, 190, 200, 32 }, "Fullscreen", button_font_);
	fullscreen_windowed_button_ = new Aegis::Button({ 580, 230, 200, 32 }, "Windowed Fullscreen", button_font_);
	toggle_vsync_button_ = new Aegis::Button({ 580, 270, 200, 32 }, "Toggle Vsync", button_font_);
	back_button_ = new Aegis::Button({ 580, 600, 200, 32 }, "Back", button_font_);

	res1280_720 = new Aegis::Button({ 580, 310, 200, 32 }, "1280x720", button_font_);
	res1600_900 = new Aegis::Button({ 580, 350, 200, 32 }, "1600x900", button_font_);
	res1920_1080 = new Aegis::Button({ 580, 390, 200, 32 }, "1920x1080", button_font_);

	vsync_ = Aegis::Application::IsVsync();
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

	windowed_button_->Render();
	fullscreen_button_->Render();
	fullscreen_windowed_button_->Render();
	toggle_vsync_button_->Render();
	back_button_->Render();
	res1280_720->Render();
	res1600_900->Render();
	res1920_1080->Render();

	Aegis::Renderer2D::EndScene();
}

void OptionsScene::OnEvent(Aegis::Event& event)
{
	auto click_event = dynamic_cast<Aegis::MouseClickEvent*>(&event);
	if (click_event) {
		if (windowed_button_->IsPressed(click_event->action_)) {
			Aegis::Application::SetFullscreen(Aegis::ScreenMode::Windowed);
		}
		if (fullscreen_button_->IsPressed(click_event->action_)) {
			Aegis::Application::SetFullscreen(Aegis::ScreenMode::Fullscreen);
		}
		if (fullscreen_windowed_button_->IsPressed(click_event->action_)) {
			Aegis::Application::SetFullscreen(Aegis::ScreenMode::FullscreenWindow);
		}
		if (toggle_vsync_button_->IsPressed(click_event->action_)) {
			if (vsync_) {
				Aegis::Application::SetVsync(false);
				vsync_ = false;
			}
			else{
				Aegis::Application::SetVsync(true);
				vsync_ = true;
			}
		}
		if (res1280_720->IsPressed(click_event->action_)) {
			SetResolution(1280, 720);
		}
		if (res1600_900->IsPressed(click_event->action_)) {
			SetResolution(1600, 900);
		}
		if (res1920_1080->IsPressed(click_event->action_)) {
			SetResolution(1920, 1080);

		}

		if (back_button_->IsPressed(click_event->action_)) {
			manager_->PopScene();
		}
	}
}

void OptionsScene::SetResolution(int x, int y)
{
	Aegis::Application::SetResolution(x, y);
	camera_.SetProjection(0, x, y, 0);
}
