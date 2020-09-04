#include "MenuScene.h"

#include "GameplayScene.h"
#include "LevelSelectScene.h"
#include "OptionsScene.h"
#include "LevelEditorScene.h"

MenuScene::MenuScene() 
{
	title_background_ = Aegis::TextureManager::Load("assets/textures/TundraBG.png");

	auto title_texture = Aegis::Texture::Create("assets/textures/TundraTitle.png"); 
	title_sprite_ = std::make_shared<Aegis::Sprite>(Aegis::Vec2(0,50), title_texture->size_, title_texture);
	Aegis::CenterAABBHorizontally(title_sprite_->rect_, {0, 0, 1280, 720});
	ui_layer_ = std::make_unique<Aegis::UILayer>();

	auto new_game_button_ = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({ 580, 170, 200, 50 }, "", [&]() {manager_->PushScene(std::unique_ptr<Scene>(new GameplayScene(0))); }));
	auto level_select_button_ = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({ 580, 220, 200, 50 }, "", [&]() {manager_->PushScene(std::unique_ptr<Scene>(new LevelSelectScene())); }));
	auto options_button_ = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({ 580, 230, 200, 50 }, "", [&]() {manager_->PushScene(std::unique_ptr<Scene>(new OptionsScene())); }));
	auto level_editor_button_ = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({ 580, 230, 200, 50 }, "", [&]() {manager_->PushScene(std::unique_ptr<Scene>(new LevelEditorScene())); }));
	auto exit_button_ = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({ 580, 270, 200, 50 }, "", [&]() { Aegis::Application::Quit(); }));

	v_box_ = ui_layer_->AddContainer({0, 170, 1280, 570}, Aegis::Container::Vertical, 10, Aegis::Container::Center);
	v_box_->AddWidget(new_game_button_);
	v_box_->AddWidget(level_select_button_);
	v_box_->AddWidget(options_button_);
	v_box_->AddWidget(level_editor_button_);
	v_box_->AddWidget(exit_button_);
	
	new_game_button_->SetStateTexture(Aegis::Button::Normal, Aegis::TextureManager::Load("assets/textures/PlayButton.png"));
	level_select_button_->SetStateTexture(Aegis::Button::Normal, Aegis::TextureManager::Load("assets/textures/LevelSelectButton.png"));
	options_button_->SetStateTexture(Aegis::Button::Normal, Aegis::TextureManager::Load("assets/textures/OptionsButton.png"));
	level_editor_button_->SetStateTexture(Aegis::Button::Normal, Aegis::TextureManager::Load("assets/textures/LevelEditorButton.png"));
	exit_button_->SetStateTexture(Aegis::Button::Normal, Aegis::TextureManager::Load("assets/textures/ExitButton.png"));
	
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
	Aegis::DrawQuad({0.0f, 0.0f}, Aegis::Application::GetWindow().GetSize(), title_background_);
	Aegis::RenderSprite(*title_sprite_);
}

void MenuScene::OnEvent(Aegis::Event& event)
{
}
