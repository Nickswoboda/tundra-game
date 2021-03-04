#include "InfoDialog.h"

InfoDialog::InfoDialog()
{
	rect_ = {50, 50, 600, 400};
	Aegis::AABB window_size{ 0, 0, Aegis::Application::GetWindow().GetSize().x, Aegis::Application::GetWindow().GetSize().y };
	Aegis::CenterAABBHorizontally(rect_, window_size);
	Aegis::CenterAABBVertically(rect_, window_size);

	v_box_ = std::make_shared<Aegis::Container>(rect_, Aegis::Container::Vertical, 10, Aegis::Alignment::HCenter);

	auto sprite_sheet = Aegis::TextureManager::Load("assets/textures/tile_map.png");

	auto bruce_info = v_box_->AddWidget<Aegis::Container>(Aegis::AABB(0,0, rect_.size.x, 64), Aegis::Container::Horizontal, 32);
	auto bruce_sprite = bruce_info->AddWidget<Aegis::SpriteWidget>(Aegis::Vec2(), sprite_sheet, Aegis::AABB(0, 96, 32, 32));
	bruce_sprite->sprite_.scale_ = {2,2};
	bruce_info->AddWidget<Aegis::Label>("You play as Bruce. Bruce slides on ice.", Aegis::Vec2());

	auto bjorn_info = v_box_->AddWidget<Aegis::Container>(Aegis::AABB(0,0, rect_.size.x, 64), Aegis::Container::Horizontal, 32);
	auto bjorn_sprite = bjorn_info->AddWidget<Aegis::SpriteWidget>(Aegis::Vec2(), sprite_sheet, Aegis::AABB(64, 96, 32, 32));
	bjorn_sprite->sprite_.scale_ = {2,2};
	bjorn_info->AddWidget<Aegis::Label>("Bjorn predicts your movements but slides on ice.", Aegis::Vec2());

	auto brutus_info = v_box_->AddWidget<Aegis::Container>(Aegis::AABB(0,0, rect_.size.x, 64), Aegis::Container::Horizontal, 32);
	auto brutus_sprite = brutus_info->AddWidget<Aegis::SpriteWidget>(Aegis::Vec2(), sprite_sheet, Aegis::AABB(32, 96, 32, 32));
	brutus_sprite->sprite_.scale_ = {2,2};
	brutus_info->AddWidget<Aegis::Label>("Brutus follows you and can change directions on ice.", Aegis::Vec2());

	auto fish_info = v_box_->AddWidget<Aegis::Container>(Aegis::AABB(0,0, rect_.size.x, 64), Aegis::Container::Horizontal, 32);
	auto fish_sprite = fish_info->AddWidget<Aegis::SpriteWidget>(Aegis::Vec2(), sprite_sheet, Aegis::AABB(96, 96, 32, 32));
	fish_sprite->sprite_.scale_ = { 2,2 };
	fish_info->AddWidget<Aegis::Label>("Collect all fish to complete the level.", Aegis::Vec2());

	close_button_ = v_box_->AddWidget<Aegis::Button>(Aegis::AABB(0, 0, 200, 50), "close");
}

void InfoDialog::Render() const
{
	Aegis::DrawQuad(rect_.pos, rect_.size, Aegis::Vec4 {0, 0, 1, 1});
	v_box_->Render();
}

void InfoDialog::OnEvent(Aegis::Event& event)
{
	v_box_->OnEvent(event);
}
