#include "MenuScene.h"

#include "GameplayScene.h"
#include "LevelSelectScene.h"
#include "OptionsScene.h"

MenuScene::MenuScene() 
{
	title_font_ = Aegis::FontManager::Instance().Load("assets/fonts/WorkSans-Regular.ttf", 64);
	button_font_ = Aegis::FontManager::Instance().Load("assets/fonts/WorkSans-Regular.ttf", 32);

	ui_layer_ = new Aegis::UILayer();
	ui_layer_->SetFont(button_font_);
	new_game_button_ = ui_layer_->AddButton({ 580, 150, 200, 32 }, "-New Game", [&]() {manager_->PushScene(std::unique_ptr<Scene>(new GameplayScene(0))); });
	level_select_button_ = ui_layer_->AddButton({ 580, 190, 200, 32 }, "-Level Select", [&]() {manager_->PushScene(std::unique_ptr<Scene>(new LevelSelectScene())); });
	options_button_ = ui_layer_->AddButton({ 580, 230, 200, 32 }, "-Options", [&]() {manager_->PushScene(std::unique_ptr<Scene>(new OptionsScene())); });
	exit_button_ = ui_layer_->AddButton({ 580, 270, 200, 32 }, "-Quit", [&]() { Aegis::Application::Quit(); });

	v_box_ = new Aegis::VContainer({0, 150, 1280, 570}, 10, Aegis::VContainer::Alignment::Left);
	v_box_->AddWidget(new_game_button_);
	v_box_->AddWidget(level_select_button_);
	v_box_->AddWidget(options_button_);
	v_box_->AddWidget(exit_button_);
}

MenuScene::~MenuScene()
{
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
}	

void MenuScene::OnEvent(Aegis::Event& event)
{
}
