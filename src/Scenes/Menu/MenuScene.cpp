#include "MenuScene.h"

#include "../Gameplay/GameplayScene.h"
#include "../LevelSelect/LevelSelectScene.h"
#include "../Options/OptionsScene.h"
#include "../LevelEditor/LevelEditorScene.h"
#include "../../Utilities.h"

#include <iostream>

MenuScene::MenuScene(GameData& game_data) 
	:game_data_(game_data)
{
	title_background_ = Aegis::TextureManager::Load("assets/textures/menu_bg.png");

	ui_layer_ = std::make_unique<Aegis::UILayer>();

	v_box_ = ui_layer_->AddWidget<Aegis::VContainer>();
	v_box_->SetPos({0, 50});
	v_box_->SetSize({1280, 720});
	v_box_->SetPadding(12);
	v_box_->SetAlignment(Aegis::Alignment::HCenter);

	auto title_texture = Aegis::Texture::Create("assets/textures/title.png");
	auto title_sprite = v_box_->AddWidget<Aegis::SpriteWidget>(Aegis::Vec2(0, 50), title_texture);

	auto new_game_button = v_box_->AddWidget<Aegis::Button>("PLAY");
	new_game_button->SetSize({ 200, 50 });
	StylizeButton(*new_game_button, 3, 32);
	new_game_button->ConnectSignal("pressed", [&]() {manager_->PushScene<GameplayScene>(1, false, game_data_);});

	auto level_select_button = v_box_->AddWidget<Aegis::Button>("LEVEL SELECT");
	level_select_button->SetSize({ 200, 50 });
	StylizeButton(*level_select_button, 3, 28);
	level_select_button->ConnectSignal("pressed", [&]() {manager_->PushScene<LevelSelectScene>(game_data_);});

	auto options_button = v_box_->AddWidget<Aegis::Button>("OPTIONS");
	options_button->SetSize({ 200, 50 });
	StylizeButton(*options_button, 3, 32);
	options_button->ConnectSignal("pressed", [&]() {manager_->PushScene<OptionsScene>(game_data_);});

	auto exit_button = v_box_->AddWidget<Aegis::Button>("EXIT");
	exit_button->SetSize({ 200, 50 });
	StylizeButton(*exit_button, 3, 32);
	exit_button->ConnectSignal("pressed", []() { Aegis::Application::Quit();});

	mute_button_ = ui_layer_->AddWidget<Aegis::Checkbox>("");
	mute_button_->SetPos({ 1200, 650 });
	mute_button_->SetSize({ 50, 50 });
	mute_button_->ConnectSignal("checked", [&](){Aegis::AudioPlayer::SetMasterVolume(0); game_data_.muted_ = true;});
	mute_button_->ConnectSignal("unchecked", [&](){Aegis::AudioPlayer::SetMasterVolume(game_data_.volume_); game_data.muted_ = false;});
	mute_button_->SetState(game_data.muted_);
	mute_button_->SetStateTexture(false, Aegis::TextureManager::Load("assets/textures/audio_on.png"));
	mute_button_->SetStateTexture(true, Aegis::TextureManager::Load("assets/textures/audio_off.png"));
	
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

void MenuScene::OnAttach()
{
	mute_button_->SetState(game_data_.muted_);
}
