#include "MenuScene.h"

#include "GameplayScene.h"
#include "LevelSelectScene.h"
#include "OptionsScene.h"
#include "LevelEditorScene.h"

#include <iostream>

void StylizeButton(Aegis::Button& button, int font_size)
{
	button.SetStateBgColor(Aegis::Button::Normal, { 0.37f, 0.5f,  0.72f, 1.0f });
	button.SetStateBgColor(Aegis::Button::Pressed, { 0.37f, 0.5f,  0.72f, 1.0f });
	button.SetStateBgColor(Aegis::Button::Hovered, { 0.45f, 0.58f, 0.78f, 1.0f });
	button.border_size_ = 3;

	button.SetFont(Aegis::FontManager::Load("assets/fonts/roboto_bold.ttf", font_size));

	auto hover_sfx = Aegis::AudioPlayer::LoadSound("assets/audio/button_hover.ogg");
	button.ConnectSignal("entered", [hover_sfx]() {Aegis::AudioPlayer::PlaySound(hover_sfx, 40); });
}

MenuScene::MenuScene(GameData& game_data) 
	:game_data_(game_data)
{
	title_background_ = Aegis::TextureManager::Load("assets/textures/menu_bg.png");

	ui_layer_ = std::make_unique<Aegis::UILayer>();
	v_box_ = ui_layer_->AddWidget<Aegis::Container>(Aegis::AABB(0, 50, 1280, 720), Aegis::Container::Vertical, 12, Aegis::Alignment::HCenter);

	auto title_texture = Aegis::Texture::Create("assets/textures/title.png");
	auto title_sprite = v_box_->AddWidget<Aegis::SpriteWidget>(Aegis::Vec2(0, 50), title_texture);

	auto new_game_button = v_box_->AddWidget<Aegis::Button>(Aegis::AABB( 580, 170, 200, 50 ), "PLAY");
	auto level_select_button = v_box_->AddWidget<Aegis::Button>(Aegis::AABB( 580, 220, 200, 50 ), "LEVEL SELECT");
	auto options_button = v_box_->AddWidget<Aegis::Button>(Aegis::AABB( 580, 230, 200, 50 ), "OPTIONS");
	auto exit_button = v_box_->AddWidget<Aegis::Button>(Aegis::AABB( 580, 270, 200, 50 ), "EXIT");

	StylizeButton(*new_game_button, 32);
	StylizeButton(*level_select_button, 28);
	StylizeButton(*options_button, 32);
	StylizeButton(*exit_button, 32);

	new_game_button->ConnectSignal("pressed", [&]() {manager_->PushScene<GameplayScene>(1, game_data_); Aegis::AudioPlayer::StopSound(bg_music_); });
	level_select_button->ConnectSignal("pressed", [&]() {manager_->PushScene<LevelSelectScene>(game_data_);});
	options_button->ConnectSignal("pressed", [&]() {manager_->PushScene<OptionsScene>();});
	exit_button->ConnectSignal("pressed", []() { Aegis::Application::Quit();});

	auto mute_button = ui_layer_->AddWidget<Aegis::Checkbox>("", Aegis::AABB(1200, 650, 50, 50));
	mute_button->ConnectSignal("checked", [](){Aegis::AudioPlayer::SetMasterVolume(0);});
	mute_button->ConnectSignal("unchecked", [](){Aegis::AudioPlayer::SetMasterVolume(100);});
	mute_button->SetTexture(false, Aegis::TextureManager::Load("assets/textures/audio_on.png"));
	mute_button->SetTexture(true, Aegis::TextureManager::Load("assets/textures/audio_off.png"));
	
	snow_engine_ = std::make_unique<ParticleEngine>(512, Aegis::Vec2(-200, -700), Aegis::Vec2(Aegis::Application::GetWindow().GetSize().x, 0), Aegis::Vec2(0.2, 2), Aegis::Vec2(1, 3));
	bg_music_ = Aegis::AudioPlayer::LoadSound("assets/audio/menu_bgm.ogg", true, true);
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
