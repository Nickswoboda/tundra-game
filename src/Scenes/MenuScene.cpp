#include "MenuScene.h"

#include "GameplayScene.h"
#include "LevelSelectScene.h"
#include "OptionsScene.h"
#include "LevelEditorScene.h"

#include <iostream>

MenuScene::MenuScene() 
{
	title_background_ = Aegis::TextureManager::Load("assets/textures/TundraBG.png");

	auto title_texture = Aegis::Texture::Create("assets/textures/TundraTitle.png"); 
	title_sprite_ = std::make_shared<Aegis::Sprite>(Aegis::Vec2(0,50), title_texture->size_, title_texture);
	Aegis::CenterAABBHorizontally(title_sprite_->rect_, {0, 0, 1280, 720});
	ui_layer_ = std::make_unique<Aegis::UILayer>();

	auto new_game_button_ = ui_layer_->AddWidget<Aegis::Button>(Aegis::AABB( 580, 170, 200, 50 ), "", [&]() {manager_->PushScene(std::unique_ptr<Scene>(new GameplayScene(1))); });
	auto level_select_button_ = ui_layer_->AddWidget<Aegis::Button>(Aegis::AABB( 580, 220, 200, 50 ), "", [&]() {manager_->PushScene(std::unique_ptr<Scene>(new LevelSelectScene())); });
	auto options_button_ = ui_layer_->AddWidget<Aegis::Button>(Aegis::AABB( 580, 230, 200, 50 ), "", [&]() {manager_->PushScene(std::unique_ptr<Scene>(new OptionsScene())); });
	auto exit_button_ = ui_layer_->AddWidget<Aegis::Button>(Aegis::AABB( 580, 270, 200, 50 ), "", [&]() { Aegis::Application::Quit(); });

	auto mute_button = ui_layer_->AddWidget<Aegis::Checkbox>("", Aegis::AABB(1200, 650, 50, 50), [](bool checked) {Aegis::AudioPlayer::SetVolume(checked ? 0 : 100);});

	mute_button->SetTexture(false, Aegis::TextureManager::Load("assets/textures/audioOn.png"));
	mute_button->SetTexture(true, Aegis::TextureManager::Load("assets/textures/audioOff.png"));

	v_box_ = ui_layer_->AddContainer({0, 170, 1280, 570}, Aegis::Container::Vertical, 10, Aegis::Container::Center);
	v_box_->AddWidget(new_game_button_);
	v_box_->AddWidget(level_select_button_);
	v_box_->AddWidget(options_button_);
	v_box_->AddWidget(exit_button_);
	
	new_game_button_->SetStateTexture(Aegis::Button::Normal, Aegis::TextureManager::Load("assets/textures/PlayButton.png"));
	level_select_button_->SetStateTexture(Aegis::Button::Normal, Aegis::TextureManager::Load("assets/textures/LevelSelectButton.png"));
	options_button_->SetStateTexture(Aegis::Button::Normal, Aegis::TextureManager::Load("assets/textures/OptionsButton.png"));
	exit_button_->SetStateTexture(Aegis::Button::Normal, Aegis::TextureManager::Load("assets/textures/ExitButton.png"));
	
	snow_engine_ = std::make_unique<ParticleEngine>(512, Aegis::Vec2(-200, -700), Aegis::Vec2(Aegis::Application::GetWindow().GetSize().x, 0), Aegis::Vec2(0.2, 2), Aegis::Vec2(1, 3));
	bg_music_ = std::make_shared<Aegis::SoundEffect>("assets/audio/bgm.ogg");
	
	Aegis::AudioPlayer::Play(bg_music_->source_id_);
}

void MenuScene::Update()
{
	snow_engine_->Update();
}

void MenuScene::Render(float delta_time)
{
	Aegis::Renderer2D::SetProjection(camera_.view_projection_matrix_);
	Aegis::DrawQuad({0.0f, 0.0f}, Aegis::Application::GetWindow().GetSize(), title_background_);
	Aegis::RenderSprite(*title_sprite_);
	snow_engine_->Render();
}

void MenuScene::OnEvent(Aegis::Event& event)
{
}
