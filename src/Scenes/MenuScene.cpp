#include "MenuScene.h"

#include "GameplayScene.h"
#include "LevelSelectScene.h"
#include "OptionsScene.h"
#include "LevelEditorScene.h"

#include <iostream>

MenuScene::MenuScene() 
{
	title_background_ = Aegis::TextureManager::Load("assets/textures/TundraBG.png");

	ui_layer_ = std::make_unique<Aegis::UILayer>();

	auto title_texture = Aegis::Texture::Create("assets/textures/TundraTitle.png");
	auto title_sprite = ui_layer_->AddWidget<Aegis::SpriteWidget>(Aegis::Vec2(0, 50), title_texture);

	auto new_game_button = ui_layer_->AddWidget<Aegis::Button>(Aegis::AABB( 580, 170, 200, 50 ), "");
	auto level_select_button = ui_layer_->AddWidget<Aegis::Button>(Aegis::AABB( 580, 220, 200, 50 ), "");
	auto options_button = ui_layer_->AddWidget<Aegis::Button>(Aegis::AABB( 580, 230, 200, 50 ), "");
	auto exit_button = ui_layer_->AddWidget<Aegis::Button>(Aegis::AABB( 580, 270, 200, 50 ), "");

	new_game_button->ConnectSignal("pressed", [&]() {manager_->PushScene(std::unique_ptr<Scene>(new GameplayScene(1))); Aegis::AudioPlayer::StopSound(bg_music_); });
	level_select_button->ConnectSignal("pressed", [&]() {manager_->PushScene(std::unique_ptr<Scene>(new LevelSelectScene()));});
	options_button->ConnectSignal("pressed", [&]() {manager_->PushScene(std::unique_ptr<Scene>(new OptionsScene()));});
	exit_button->ConnectSignal("pressed", []() { Aegis::Application::Quit();});

	new_game_button->ConnectSignal("entered", [&]() {Aegis::AudioPlayer::PlaySound(button_hover_sfx_, 40); });
	level_select_button->ConnectSignal("entered", [&]() {Aegis::AudioPlayer::PlaySound(button_hover_sfx_, 40); });
	options_button->ConnectSignal("entered", [&]() {Aegis::AudioPlayer::PlaySound(button_hover_sfx_, 40); });
	exit_button->ConnectSignal("entered", [&]() {Aegis::AudioPlayer::PlaySound(button_hover_sfx_, 40); });

	auto mute_button = ui_layer_->AddWidget<Aegis::Checkbox>("", Aegis::AABB(1200, 650, 50, 50));
	mute_button->ConnectSignal("checked", [](){Aegis::AudioPlayer::SetMasterVolume(0);});
	mute_button->ConnectSignal("unchecked", [](){Aegis::AudioPlayer::SetMasterVolume(100);});
	mute_button->SetTexture(false, Aegis::TextureManager::Load("assets/textures/audioOn.png"));
	mute_button->SetTexture(true, Aegis::TextureManager::Load("assets/textures/audioOff.png"));

	v_box_ = ui_layer_->AddContainer({0, 50, 1280, 720}, Aegis::Container::Vertical, 12, Aegis::Container::Center);
	v_box_->AddWidget(title_sprite);
	v_box_->AddWidget(new_game_button);
	v_box_->AddWidget(level_select_button);
	v_box_->AddWidget(options_button);
	v_box_->AddWidget(exit_button);
	
	new_game_button->SetStateTexture(Aegis::Button::Normal, Aegis::TextureManager::Load("assets/textures/PlayButton.png"));
	new_game_button->SetStateTexture(Aegis::Button::Hovered, Aegis::TextureManager::Load("assets/textures/play-btn-hovered.png"));
	level_select_button->SetStateTexture(Aegis::Button::Normal, Aegis::TextureManager::Load("assets/textures/LevelSelectButton.png"));
	level_select_button->SetStateTexture(Aegis::Button::Hovered, Aegis::TextureManager::Load("assets/textures/level-select-btn-hovered.png"));
	options_button->SetStateTexture(Aegis::Button::Normal, Aegis::TextureManager::Load("assets/textures/OptionsButton.png"));
	options_button->SetStateTexture(Aegis::Button::Hovered, Aegis::TextureManager::Load("assets/textures/options-btn-hovered.png"));
	exit_button->SetStateTexture(Aegis::Button::Normal, Aegis::TextureManager::Load("assets/textures/ExitButton.png"));
	exit_button->SetStateTexture(Aegis::Button::Hovered, Aegis::TextureManager::Load("assets/textures/exit-btn-hovered.png"));
	
	snow_engine_ = std::make_unique<ParticleEngine>(512, Aegis::Vec2(-200, -700), Aegis::Vec2(Aegis::Application::GetWindow().GetSize().x, 0), Aegis::Vec2(0.2, 2), Aegis::Vec2(1, 3));
	button_hover_sfx_ = Aegis::AudioPlayer::LoadSound("assets/audio/button-hover.ogg");
	bg_music_ = Aegis::AudioPlayer::LoadSound("assets/audio/menu-bgm.ogg", true, true);
	Aegis::AudioPlayer::PlaySound(bg_music_, 100);
}

void MenuScene::Update()
{
	snow_engine_->Update();
}

void MenuScene::Render(float delta_time)
{
	Aegis::Renderer2D::SetProjection(camera_.view_projection_matrix_);
	Aegis::DrawQuad({0.0f, 0.0f}, Aegis::Application::GetWindow().GetSize(), *title_background_);
	snow_engine_->Render();
}

void MenuScene::OnEvent(Aegis::Event& event)
{
}
