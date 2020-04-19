#include "GameObject.h"

Aegis::Vec2 LERP(const Aegis::Vec2& a, const Aegis::Vec2& b, float percentage)
{
	if (percentage >= 1.0f) {
		return b;
	}
	Aegis::Vec2 temp = a * (1.0f - percentage) + b * percentage;
	return temp;
}

Aegis::Vec2	EaseInQuad(const Aegis::Vec2& a, const Aegis::Vec2& b, float percentage)
{
	if (percentage >= 1.0f) {
		return b;
	}
	float t = percentage * percentage;
	Aegis::Vec2 temp = a * (1.0f - t) + b * t;
	return temp;
}
Aegis::Vec2	EaseInEaseOutQuad(const Aegis::Vec2& a, const Aegis::Vec2& b, float percentage)
{
	if (percentage >= 1.0f) {
		return b;
	}
	float t = percentage < 0.5 ? 2 * percentage * percentage : 1 - pow(-2 * percentage + 2, 2) / 2;
	Aegis::Vec2 temp = a * (1.0f - t) + b * t;
	return temp;
}
Aegis::Vec2	EaseInEaseOutQuart(const Aegis::Vec2& a, const Aegis::Vec2& b, float percentage)
{
	if (percentage >= 1.0f) {
		return b;
	}

	float t = percentage < 0.5 ? 8 * percentage * percentage * percentage * percentage : 1 - pow(-2 * percentage + 2, 4) / 2;
	Aegis::Vec2 temp = a * (1.0f - t) + b * t;
	return temp;
}


void Player::Render(float delta_time) const
{
	Aegis::RenderSprite(sprite_);
	Aegis::DrawQuad(grid_coord_ * 32, rect_.size, { 0.0f, 1.0f, 0.0f, 0.5f });
}

void GameObject::StartMoving()
{
	animation_.playing_ = true;

	Aegis::Vec2 prev_tile_pos = rect_.pos / 32;
	Aegis::Vec2 vec = grid_coord_ - prev_tile_pos;
	int num_tiles = sqrt(vec.x * vec.x + vec.y * vec.y);

	animation_.duration_ = speed_ * num_tiles;
	animation_.timer_.Start();
	animation_.start_point_ = rect_.pos;
}


void Enemy::Render(float delta_time) const
{
	Aegis::RenderSprite(sprite_);
	Aegis::DrawQuad(grid_coord_ * 32, rect_.size, { 1.0f, 0.0f, 0.0f, 0.5f });
}

void GameObject::Update()
{
	animation_.timer_.Update();
	float percentage = animation_.timer_.GetElapsedInSeconds() / animation_.duration_;

	rect_.pos = LERP(animation_.start_point_, grid_coord_ * 32, percentage);;
	sprite_.pos_ = rect_.pos;

	if (rect_.pos == grid_coord_ * 32) {
		sprite_.pos_ = rect_.pos;
		animation_.playing_ = false;
		animation_.timer_.Stop();
	}
}

void GameObject::SetPosition(Aegis::Vec2 pos)
{
	rect_.pos = pos;
	sprite_.pos_ = pos;
}
