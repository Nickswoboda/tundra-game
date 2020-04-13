#include "GameObject.h"

void Player::Update()
{
	rect_.pos.x += vel_.x;
	rect_.pos.y += vel_.y;
}

void Player::Stop()
{
	vel_.x = 0;
	vel_.y = 0;

	moving_ = false;
}

void Player::Render(float delta_time) const
{
	Aegis::DrawQuad(rect_.pos, rect_.size, texture_, { 1.0f, 1.0f, 1.0f, 1.0f }, uv_coords_);
}