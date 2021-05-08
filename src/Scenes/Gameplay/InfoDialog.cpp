#include "InfoDialog.h"
#include "../../Utilities.h"

InfoDialog::InfoDialog()
{
	AddSignal("closed");
	rect_ = {0, 0, 800, 600};
	Aegis::CenterAABB(rect_, Aegis::Application::GetWindow().GetViewport());

	bg_ = Aegis::TextureManager::Load("assets/textures/info_frame.png");

	v_box_.SetPos(rect_.pos + Aegis::Vec2(0, 50));
	v_box_.SetSize(rect_.size - Aegis::Vec2(0, 50));
	v_box_.SetPadding(10);
	v_box_.SetAlignment(Aegis::Alignment::HCenter);

	auto sprite_sheet = Aegis::TextureManager::Load("assets/textures/tile_map.png");

	Aegis::Vec2 info_size = {650, 64};
	auto bruce_info = v_box_.AddWidget<Aegis::HContainer>();
	bruce_info->SetSize(info_size);
	bruce_info->SetPadding(32);
	bruce_info->SetAlignment(Aegis::Alignment::VCenter);

	auto bruce_sprite = bruce_info->AddWidget<Aegis::SpriteWidget>(Aegis::Vec2(), sprite_sheet, Aegis::AABB(0, 96, 32, 32));
	bruce_sprite->SetScale({2.0f, 2.0f});
	auto bruce_text = bruce_info->AddWidget<Aegis::VContainer>();
	bruce_text->SetSize({500, 64});
	bruce_text->SetAlignment(Aegis::Alignment::VCenter);
	bruce_text->AddWidget<Aegis::Label>("You play as Bruce.", Aegis::Vec2());
	bruce_text->AddWidget<Aegis::Label>("You are unable to change directions on ice.", Aegis::Vec2());
	bruce_text->AddWidget<Aegis::Label>("You must use a wall or ground tiles to change directions.", Aegis::Vec2());

	auto fish_info = v_box_.AddWidget<Aegis::HContainer>();
	fish_info->SetSize(info_size);
	fish_info->SetPadding(32);
	fish_info->SetAlignment(Aegis::Alignment::VCenter);
	auto fish_sprite = fish_info->AddWidget<Aegis::SpriteWidget>(Aegis::Vec2(), sprite_sheet, Aegis::AABB(96, 96, 32, 32));
	fish_sprite->SetScale({2.0f, 2.0f});
	fish_info->AddWidget<Aegis::Label>("Collect all the fish while avoiding the bears to complete the level.", Aegis::Vec2());

	auto star_info = v_box_.AddWidget<Aegis::HContainer>();
	star_info->SetSize(info_size);
	star_info->SetPadding(32);
	star_info->SetAlignment(Aegis::Alignment::VCenter);
	auto star_sprite = star_info->AddWidget<Aegis::SpriteWidget>(Aegis::Vec2(), sprite_sheet, Aegis::AABB(0, 128, 32, 32));
	star_sprite->SetScale({2.0f, 2.0f});
	auto star_text = star_info->AddWidget<Aegis::VContainer>();
	star_text->SetSize({505, 64});
	star_text->SetAlignment(Aegis::Alignment::VCenter);
	star_text->AddWidget<Aegis::Label>("Earn stars for each level you complete.", Aegis::Vec2());
	star_text->AddWidget<Aegis::Label>("The quicker the level is beaten, the more stars earned (up to 3).", Aegis::Vec2());

	auto bjorn_info = v_box_.AddWidget<Aegis::HContainer>();
	bjorn_info->SetSize(info_size);
	bjorn_info->SetPadding(32);
	bjorn_info->SetAlignment(Aegis::Alignment::VCenter);
	auto bjorn_sprite = bjorn_info->AddWidget<Aegis::SpriteWidget>(Aegis::Vec2(), sprite_sheet, Aegis::AABB(64, 96, 32, 32));
	bjorn_sprite->SetScale({2.0f, 2.0f});
	bjorn_info->AddWidget<Aegis::Label>("Bjorn is quick and predicts your movements but slides on ice.", Aegis::Vec2());

	auto brutus_info = v_box_.AddWidget<Aegis::HContainer>();
	brutus_info->SetSize(info_size);
	brutus_info->SetPadding(32);
	brutus_info->SetAlignment(Aegis::Alignment::VCenter);
	auto brutus_sprite = brutus_info->AddWidget<Aegis::SpriteWidget>(Aegis::Vec2(), sprite_sheet, Aegis::AABB(32, 96, 32, 32));
	brutus_sprite->SetScale({2.0f, 2.0f});
	brutus_info->AddWidget<Aegis::Label>("Brutus is slower and follows you, but can turn on ice.", Aegis::Vec2());

	v_box_.AddWidget<Aegis::Label>("Press the arrow or 'WASD' keys to move.", Aegis::Vec2());
	v_box_.AddWidget<Aegis::Label>("Press 'ESC' to pause the game.", Aegis::Vec2());

	Aegis::AABB button_rect = {0, rect_.pos.y + rect_.size.y - 96, 200, 50};
	Aegis::CenterAABBHorizontally(button_rect, rect_);
	close_button_ = std::make_shared<Aegis::Button>(button_rect, "Close");
	close_button_->ConnectSignal("pressed", [&]() { visible_ = false; Emit("closed"); });
	StylizeButton(*close_button_, 3, 32);
}

void InfoDialog::Render() const
{
	Aegis::DrawQuad(rect_.pos, *bg_);
	v_box_.Render();
	close_button_->Render();
}

void InfoDialog::OnEvent(Aegis::Event& event)
{
	if (visible_) {
		close_button_->OnEvent(event);
		event.handled_ = true;
	}
}
