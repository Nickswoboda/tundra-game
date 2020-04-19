#include "GameObject.h"

void Player::Render(float delta_time) const
{
	Aegis::RenderSprite(sprite_);
	Aegis::DrawQuad(grid_coord_ * 32, rect_.size, { 0.0f, 1.0f, 0.0f, 0.5f });
}

void GameObject::StartMoving()
{
	Aegis::Vec2 prev_tile_pos = rect_.pos / 32;
	Aegis::Vec2 vec = grid_coord_ - prev_tile_pos;
	int num_tiles = sqrt(vec.x * vec.x + vec.y * vec.y);

	animation_.Start(rect_.pos, grid_coord_ * 32, speed_ * num_tiles);
}


void Enemy::Render(float delta_time) const
{
	Aegis::RenderSprite(sprite_);
	Aegis::DrawQuad(grid_coord_ * 32, rect_.size, { 1.0f, 0.0f, 0.0f, 0.5f });
}

void GameObject::Update()
{
	animation_.Update();
	rect_.pos = animation_.current_value_;
	sprite_.pos_ = rect_.pos;

	if (rect_.pos == grid_coord_ * 32) {
		animation_.Stop();
	}
}

void GameObject::SetPosition(Aegis::Vec2 pos)
{
	rect_.pos = pos;
	sprite_.pos_ = pos;
}

void Animation::Start(Aegis::Vec2 start, Aegis::Vec2 end, float duration)
{
	start_value_ = start;
	end_value_ = end;
	duration_ = duration;
	playing_ = true;
	timer_.Start();
}

void Animation::Update()
{
	timer_.Update();
	float percentage = timer_.GetElapsedInSeconds() / duration_;
	current_value_ = Aegis::LERP(start_value_, end_value_, percentage);
}

void Animation::Stop()
{
	playing_ = false;
	timer_.Stop();
}
