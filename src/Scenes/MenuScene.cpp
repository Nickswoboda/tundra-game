#include "MenuScene.h"

#include "../FontManager.h"
#include "GameplayScene.h"
#include "LevelSelectScene.h"

MenuScene::MenuScene() 
	:camera_(0, 1280, 720, 0)
{
	title_font_ = FontManager::Instance().Load("assets/fonts/WorkSans-Regular.ttf", 64);
	button_font_ = FontManager::Instance().Load("assets/fonts/WorkSans-Regular.ttf", 32);

	new_game_button_ = new Button({ 580, 150, 200, 32 }, "-New Game", button_font_);
	level_select_button_ = new Button({ 580, 190, 200, 32 }, "-Level Select", button_font_);
	options_button_ = new Button({ 580, 230, 200, 32 }, "-Options", button_font_);
	exit_button_ = new Button({ 580, 270, 200, 32 }, "-Quit", button_font_);
}

MenuScene::~MenuScene()
{
	delete new_game_button_;
	delete level_select_button_;
	delete options_button_;
	delete exit_button_;
}

void MenuScene::Update()
{
}

void MenuScene::Render(float delta_time)
{
	Aegis::Renderer2D::BeginScene(camera_.view_projection_matrix_);
	Aegis::RendererClear();

	Aegis::Renderer2D::SetDefaultFont(title_font_);
	Aegis::DrawText("TUNDRA", { 540, 70 }, { 1.0, 0.0, 0.0, 1.0f });

	new_game_button_->Render();
	level_select_button_->Render();
	options_button_->Render();
	exit_button_->Render();

	Aegis::Renderer2D::EndScene();
}	

void MenuScene::OnEvent(Aegis::Event& event)
{
	auto click_event = dynamic_cast<Aegis::MouseClickEvent*>(&event);

	if (click_event) {

		if (new_game_button_->IsPressed(click_event->action_)) {
			manager_->PushScene(std::unique_ptr<Scene>(new GameplayScene(0)));
		}

		if (level_select_button_->IsPressed(click_event->action_)) {
			manager_->PushScene(std::unique_ptr<Scene>(new LevelSelectScene()));
		}

		if (exit_button_->IsPressed(click_event->action_)) {
			Aegis::Application::Quit();
		}
	}

}
