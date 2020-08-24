#include "MenuScene.h"

#include "GameplayScene.h"
#include "LevelSelectScene.h"
#include "OptionsScene.h"

MenuScene::MenuScene() 
{
	title_font_ = Aegis::Font::Create("assets/fonts/WorkSans-Regular.ttf", 64);
	button_font_ = Aegis::FontManager::Instance().Load("assets/fonts/WorkSans-Regular.ttf", 32);

	title_background_ = Aegis::TextureManager::Instance().Load("assets/textures/TundraBG.png");

	auto title_text_texture = Aegis::Texture::TextureFromText("TUNDRA", title_font_);
	title_sprite_ = std::make_shared<Aegis::Sprite>(Aegis::Vec2(0,50), title_text_texture->size_, title_text_texture);
	Aegis::CenterAABBHorizontally(title_sprite_->rect_, {0, 0, 1280, 720});
	ui_layer_ = std::make_unique<Aegis::UILayer>();
	ui_layer_->SetFont(button_font_);
	new_game_button_ = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({ 580, 150, 125, 50 }, "", [&]() {manager_->PushScene(std::unique_ptr<Scene>(new GameplayScene(0))); }));
	level_select_button_ = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({ 580, 190, 200, 32 }, "-Level Select", [&]() {manager_->PushScene(std::unique_ptr<Scene>(new LevelSelectScene())); }));
	options_button_ = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({ 580, 230, 200, 32 }, "-Options", [&]() {manager_->PushScene(std::unique_ptr<Scene>(new OptionsScene())); }));
	exit_button_ = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({ 580, 270, 200, 32 }, "-Quit", [&]() { Aegis::Application::Quit(); }));

	v_box_ = ui_layer_->AddContainer({0, 150, 1280, 570}, Aegis::Container::Vertical, 10, Aegis::Container::Center);
	v_box_->AddWidget(new_game_button_);
	v_box_->AddWidget(level_select_button_);
	v_box_->AddWidget(options_button_);
	v_box_->AddWidget(exit_button_);
	
	new_game_button_->SetStateTexture(Aegis::Button::Normal, Aegis::TextureManager::Instance().Load("assets/textures/normalbtn.png"));
	new_game_button_->SetStateTexture(Aegis::Button::Hovered, Aegis::TextureManager::Instance().Load("assets/textures/hoveredbtn.png"));
	new_game_button_->SetStateTexture(Aegis::Button::Pressed, Aegis::TextureManager::Instance().Load("assets/textures/pressedbtn.png"));
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
