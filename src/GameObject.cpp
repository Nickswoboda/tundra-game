#include "GameObject.h"

void Player::Update()
{
	rect_.pos += vel_ * acceleration_;
	sprite_.pos_ = rect_.pos;
}

void Player::Render(float delta_time) const
{
	Aegis::RenderSprite(sprite_);
}

void Enemy::Update()
{
	rect_.pos += vel_;
	sprite_.pos_ = rect_.pos;
}

void Enemy::Render(float delta_time) const
{
	Aegis::RenderSprite(sprite_);
	Aegis::DrawQuad(target_pos_, rect_.size, { 1.0f, 0.0f, 0.0f, 0.5f });
}

void GameObject::SetPosition(Aegis::Vec2 pos)
{
	rect_.pos = pos;
	sprite_.pos_ = pos;
}
