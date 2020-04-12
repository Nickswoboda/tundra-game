#include "GameObject.h"

void Player::Update()
{
	if (moving_) {
		rect_.pos.x += vel_.x;
		rect_.pos.y += vel_.y;
	}
}

void Player::Stop()
{
	vel_.x = 0;
	vel_.y = 0;

	moving_ = false;

	if (queued_event_) {
		OnEvent(*queued_event_);
		queued_event_.release();
	}
}

void Player::OnEvent(Aegis::Event& event)
{
	auto key_event = dynamic_cast<Aegis::KeyEvent*>(&event);
	if (key_event) {

		if (moving_ && key_event->action_ == AE_BUTTON_PRESS) {
			queued_event_ = std::make_unique<Aegis::KeyEvent>(*key_event);
		}

		if ((!vel_.x && !vel_.y) && key_event->action_ == AE_BUTTON_PRESS) {
			switch (key_event->key_) {
			case AE_KEY_UP: moving_ = true; vel_.y -= acceleration_; break;
			case AE_KEY_DOWN: moving_ = true; vel_.y += acceleration_; break;
			case AE_KEY_LEFT: moving_ = true; vel_.x -= acceleration_; break;
			case AE_KEY_RIGHT:moving_ = true; vel_.x += acceleration_; break;
			}
		}
	}
}

void Player::Render(float delta_time) const
{
	Aegis::DrawQuad(rect_.pos, rect_.size, texture_, { 1.0f, 1.0f, 1.0f, 1.0f }, uv_coords_);
}