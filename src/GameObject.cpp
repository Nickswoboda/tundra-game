#include "GameObject.h"

Aegis::Vec2 LERP(const Aegis::Vec2& a, const Aegis::Vec2& b, float percentage)
{
	Aegis::Vec2 temp = a * (1.0f - percentage) + b * percentage;
	return temp;
}

Aegis::Vec2	EaseInQuad(float percentage, float elapsed, const Aegis::Vec2& a, const Aegis::Vec2& b, float total_duration)
{
	float t = percentage * percentage;
	Aegis::Vec2 temp = a * (1.0f - t) + b * t;
	return temp;
}
Aegis::Vec2	EaseInEaseOutQuad(float percentage, float elapsed, const Aegis::Vec2& a, const Aegis::Vec2& b, float total_duration)
{
	float t = percentage < 0.5 ? 2 * percentage * percentage : 1 - pow(-2 * percentage + 2, 2) / 2;
	Aegis::Vec2 temp = a * (1.0f - t) + b * t;
	return temp;
}
Aegis::Vec2	EaseInEaseOutQuart(float percentage,const Aegis::Vec2& a, const Aegis::Vec2& b)
{
	float t = percentage < 0.5 ? 8 * percentage * percentage * percentage * percentage : 1 - pow(-2 * percentage + 2, 4) / 2;
	Aegis::Vec2 temp = a * (1.0f - t) + b * t;
	return temp;
}


void Player::Update()
{
	anim_timer_.Update();
	float timer_sec = anim_timer_.GetElapsedInSeconds();
	float percentage =timer_sec / total_anim_time_;

	Aegis::Vec2 lerp = LERP(start_point_, grid_coord_ * 32, percentage);
	Aegis::Vec2 ease_in_q = EaseInQuad(percentage, anim_timer_.elapsed_time_, start_point_, grid_coord_ * 32, total_anim_time_);
	Aegis::Vec2 ease_in_out_q = EaseInEaseOutQuad(percentage, anim_timer_.elapsed_time_, start_point_, grid_coord_ * 32, total_anim_time_);
	Aegis::Vec2 ease_in_out_qrt = EaseInEaseOutQuart(percentage, start_point_, grid_coord_ * 32);
	rect_.pos = lerp;
	sprite_.pos_ = rect_.pos;

	if (percentage >= 1.0f){
		rect_.pos = grid_coord_ * 32;
		sprite_.pos_ = rect_.pos;
		moving_ = false;
		anim_timer_.Stop();
	}

}

void Player::Render(float delta_time) const
{
	Aegis::RenderSprite(sprite_);
	Aegis::DrawQuad(grid_coord_ * 32, rect_.size, { 0.0f, 1.0f, 0.0f, 0.5f });
}

void GameObject::StartMoving()
{
	moving_ = true;

	Aegis::Vec2 prev_tile_pos = rect_.pos / 32;
	Aegis::Vec2 vec = grid_coord_ - prev_tile_pos;
	int num_tiles = sqrt(vec.x * vec.x + vec.y * vec.y);
	total_anim_time_ = anim_time_per_tile_ * num_tiles;
	anim_timer_.Start();
	start_point_ = rect_.pos;
}

void Enemy::Update()
{
	anim_timer_.Update();
	float timer_sec = anim_timer_.GetElapsedInSeconds();
	float percentage = timer_sec / total_anim_time_;

	Aegis::Vec2 lerp = LERP(start_point_, grid_coord_ * 32, percentage);
	Aegis::Vec2 ease_in_q = EaseInQuad(percentage, anim_timer_.elapsed_time_, start_point_, grid_coord_ * 32, total_anim_time_);
	Aegis::Vec2 ease_in_out_q = EaseInEaseOutQuad(percentage, anim_timer_.elapsed_time_, start_point_, grid_coord_ * 32, total_anim_time_);
	Aegis::Vec2 ease_in_out_qrt = EaseInEaseOutQuart(percentage, start_point_, grid_coord_ * 32);
	rect_.pos = lerp;
	sprite_.pos_ = rect_.pos;

	if (percentage >= 1.0f) {
		rect_.pos = grid_coord_ * 32;
		sprite_.pos_ = rect_.pos;
		moving_ = false;
		anim_timer_.Stop();
	}
}

void Enemy::Render(float delta_time) const
{
	Aegis::RenderSprite(sprite_);
	Aegis::DrawQuad(grid_coord_ * 32, rect_.size, { 1.0f, 0.0f, 0.0f, 0.5f });
}

void GameObject::SetPosition(Aegis::Vec2 pos)
{
	rect_.pos = pos;
	sprite_.pos_ = pos;
}
