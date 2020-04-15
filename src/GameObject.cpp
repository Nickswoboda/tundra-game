#include "GameObject.h"

void Player::Update()
{
	rect_.pos += vel_;
}

void Player::Render(float delta_time) const
{
	Aegis::DrawQuad(rect_.pos, rect_.size, texture_, { 1.0f, 1.0f, 1.0f, 1.0f }, uv_coords_);
}