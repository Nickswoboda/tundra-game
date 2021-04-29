#include "OptionsScene.h"

#include "../../Utilities.h"

OptionsScene::OptionsScene(GameData& game_data)
{
	auto title_font_ = Aegis::FontManager::Load("assets/fonts/roboto_bold.ttf", 64);

	ui_layer_ = std::make_unique<Aegis::UILayer>();

	bg_texture_ = Aegis::TextureManager::Load("assets/textures/scene_bg.png");

	auto viewport = Aegis::Application::GetWindow().GetViewport();
	auto rect = Aegis::AABB{0, 0, 250, 400};
	Aegis::CenterAABB(rect, viewport);
	v_box_ = ui_layer_->AddWidget<Aegis::Container>(rect, Aegis::Container::Vertical, 16, Aegis::Alignment::HCenter);

	auto title_label = ui_layer_->AddWidget<Aegis::Label>("Options", Aegis::Vec2(0, 16));
	title_label->SetFont(title_font_);
	auto label_rect = title_label->GetRect();
	Aegis::CenterAABBHorizontally(label_rect, viewport);
	title_label->SetPos(label_rect.pos);

	auto volume_box = v_box_->AddWidget<Aegis::Container>(Aegis::AABB(0,0, 250, 40), Aegis::Container::Horizontal, 8, Aegis::Alignment::Center);
	auto volume_label = volume_box->AddWidget<Aegis::Label>("Volume:", Aegis::Vec2());
	volume_label->SetFont(Aegis::FontManager::Load("assets/fonts/roboto_bold.ttf", 32));

	auto volume_spinbox = volume_box->AddWidget<Aegis::SpinBox>(100, 10);
	volume_spinbox->max_value_ = 100;
	StylizeSpinBox(*volume_spinbox, 3, 16);
	volume_spinbox->SetSuffix("%");
	volume_spinbox->SetValue(game_data.volume_);
	volume_spinbox->ConnectSignal("value changed", [&, volume_spinbox](){
			int volume = volume_spinbox->GetValue();
			game_data.volume_ = volume;
			if (!game_data.muted_){
				Aegis::AudioPlayer::SetMasterVolume(volume);
			} });

	auto checkbox_box = v_box_->AddWidget<Aegis::Container>(Aegis::AABB(0,0, 150, 250), Aegis::Container::Vertical, 24);

	auto mute_checkbox = checkbox_box->AddWidget<Aegis::Checkbox>("Mute");
	mute_checkbox->ConnectSignal("checked", [&game_data](){Aegis::AudioPlayer::SetMasterVolume(0); game_data.muted_ = true;});
	mute_checkbox->ConnectSignal("unchecked", [&game_data](){Aegis::AudioPlayer::SetMasterVolume(game_data.volume_); game_data.muted_ = false;});
	mute_checkbox->SetState(game_data.muted_);
	StylizeCheckbox(*mute_checkbox, 3, 32);

	auto vsync_checkbox = checkbox_box->AddWidget<Aegis::Checkbox>("Vsync");
	vsync_checkbox->ConnectSignal("checked", [&game_data](){Aegis::Application::GetWindow().SetVsync(true); game_data.vsync_ = true;});
	vsync_checkbox->ConnectSignal("unchecked", [&game_data](){Aegis::Application::GetWindow().SetVsync(false); game_data.vsync_ = false;});
	vsync_checkbox->SetState(game_data.vsync_);
	StylizeCheckbox(*vsync_checkbox, 3, 32);

	auto back_button_ = v_box_->AddWidget<Aegis::Button>(Aegis::AABB( 580, 600, 150, 50 ), "Back");
	back_button_->ConnectSignal("pressed", [&](){manager_->PopScene();});
	StylizeButton(*back_button_, 3, 16);
}

OptionsScene::~OptionsScene()
{
}

void OptionsScene::Update()
{
}

void OptionsScene::Render(float delta_time)
{
	Aegis::Renderer2D::SetProjection(camera_.view_projection_matrix_);

	Aegis::DrawQuad({0,0}, *bg_texture_);
	auto& rect = v_box_->GetRect();
	DrawFrameBorder(rect);
	Aegis::DrawQuad(rect.pos, rect.size, k_bg_color);

}

void OptionsScene::OnEvent(Aegis::Event& event)
{
}
