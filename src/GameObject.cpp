#include "GameObject.h"

void Player::Update()
{
	pos_.x += vel_.x;
	pos_.y += vel_.y;
}

void Player::OnEvent(Aegis::Event& event)
{
	auto key_event = dynamic_cast<Aegis::KeyEvent*>(&event);
	if (key_event) {

		if ((!vel_.x && !vel_.y) && key_event->action_ == GLFW_PRESS) {
			switch (key_event->key_) {
			case GLFW_KEY_UP: vel_.y -= acceleration_; break;
			case GLFW_KEY_DOWN: vel_.y += acceleration_; break;
			case GLFW_KEY_LEFT: vel_.x -= acceleration_; break;
			case GLFW_KEY_RIGHT: vel_.x += acceleration_; break;
			}
		}
	}
}

void Player::Render(float delta_time) const
{
	Aegis::DrawQuad(pos_, size_, color_);
}