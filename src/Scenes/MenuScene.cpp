#include "MenuScene.h"

#include "GameplayScene.h"
#include "LevelSelectScene.h"
#include "OptionsScene.h"

MenuScene::MenuScene() 
{
	title_font_ = Aegis::FontManager::Instance().Load("assets/fonts/WorkSans-Regular.ttf", 64);
	button_font_ = Aegis::FontManager::Instance().Load("assets/fonts/WorkSans-Regular.ttf", 32);

	ui_layer_ = new Aegis::UILayer();
	new_game_button_ = ui_layer_->AddButton({ 580, 150, 200, 32 }, "-New Game", button_font_, [&]() {manager_->PushScene(std::unique_ptr<Scene>(new GameplayScene(0))); });
	level_select_button_ = ui_layer_->AddButton({ 580, 190, 200, 32 }, "-Level Select", button_font_, [&]() {manager_->PushScene(std::unique_ptr<Scene>(new LevelSelectScene())); });
	options_button_ = ui_layer_->AddButton({ 580, 230, 200, 32 }, "-Options", button_font_, [&]() {manager_->PushScene(std::unique_ptr<Scene>(new OptionsScene())); });
	exit_button_ = ui_layer_->AddButton({ 580, 270, 200, 32 }, "-Quit", button_font_, [&]() { Aegis::Application::Quit(); });
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
	Aegis::Renderer2D::SetProjection(camera_.view_projection_matrix_);
	Aegis::RendererClear();

	Aegis::Renderer2D::SetFont(title_font_);

	Aegis::DrawText("TUNDRA", { 540, 70 + (10 * 1) }, { 1.0, 1.0, 1.0, 1.0f });

	new_game_button_->Render();
	level_select_button_->Render();
	options_button_->Render();
	exit_button_->Render();
}	

void MenuScene::OnEvent(Aegis::Event& event)
{
}
