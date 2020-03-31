#include "Player.h"

void Player::OnUpdate()
{
	x_pos_ += x_vel_;
	y_pos_ += y_vel_;
}

void Player::OnEvent(Aegis::Event& event)
{
	auto key_event = dynamic_cast<Aegis::KeyEvent*>(&event);
	if (key_event) {

		if ((!x_vel_ && !y_vel_ ) && key_event->action_ == GLFW_PRESS) {
			switch (key_event->key_) {
			case GLFW_KEY_UP: y_vel_ -= acceleration_; break;
			case GLFW_KEY_DOWN: y_vel_ += acceleration_; break;
			case GLFW_KEY_LEFT: x_vel_ -= acceleration_; break;
			case GLFW_KEY_RIGHT: x_vel_ += acceleration_; break;
			}
		}
	}
}

void Player::OnRender()
{
	Aegis::Renderer2D::DrawQuad({ x_pos_, y_pos_ }, { width_, height_ }, color_);
}
