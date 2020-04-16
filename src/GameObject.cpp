#include "GameObject.h"

void Player::Update()
{
	rect_.pos += vel_;
}

void Player::Render(float delta_time) const
{
	Aegis::DrawQuad(rect_.pos, rect_.size, texture_, { 1.0f, 1.0f, 1.0f, 1.0f }, uv_coords_);
}

void Enemy::Update()
{
	rect_.pos += vel_;
}

void Enemy::Render(float delta_time) const
{
	Aegis::DrawQuad(rect_.pos, rect_.size, texture_, { 1.0f, 1.0f, 1.0f, 1.0f }, uv_coords_);
	Aegis::DrawQuad(target_pos_, rect_.size, { 1.0f, 0.0f, 0.0f, 0.5f });
}
