#include "GameObject.h"

static const int TILE_SIZE = 32;

void GameObject::StartMoving()
{
	Aegis::Vec2 prev_tile_pos = rect_.pos / TILE_SIZE;
	Aegis::Vec2 vec = grid_coord_ - prev_tile_pos;
	int num_tiles = sqrt(vec.x * vec.x + vec.y * vec.y);

	animation_.Start(rect_.pos, grid_coord_ * TILE_SIZE, speed_ * num_tiles);
}

void Player::Render(float delta_time) const
{
	Aegis::RenderSprite(sprite_);
	//Aegis::DrawQuad(grid_coord_ * TILE_SIZE, rect_.size, { 0.0f, 1.0f, 0.0f, 0.5f });
}

void Brutus::Render(float delta_time) const
{
	Aegis::RenderSprite(sprite_);
	//Aegis::DrawQuad(grid_coord_ * TILE_SIZE, rect_.size, { 1.0f, 0.0f, 0.0f, 0.5f });
}

void Bjorne::Render(float delta_time) const
{
	Aegis::RenderSprite(sprite_);
	//Aegis::DrawQuad(grid_coord_ * TILE_SIZE, rect_.size, { 1.0f, 1.0f, 0.0f, 0.5f });
}

void GameObject::Update()
{
	animation_.Update();
	rect_.pos = animation_.current_value_;
	sprite_.rect_.pos = rect_.pos;

	if (rect_.pos == grid_coord_ * TILE_SIZE) {
		animation_.Stop();
	}
}

void GameObject::SetPosition(Aegis::Vec2 pos)
{
	rect_.pos = pos;
	sprite_.rect_.pos = pos;
}

void Animation::Start(Aegis::Vec2 start, Aegis::Vec2 end, float duration)
{
	start_value_ = start;
	end_value_ = end;
	total_frames = duration / (1/60.0f);
	playing_ = true;
}

void Animation::Update()
{
	++current_frame;
	float percentage = current_frame / (float)total_frames;
	current_value_ = Aegis::LERP(start_value_, end_value_, percentage);
}

void Animation::Stop()
{
	playing_ = false;
	current_frame = 0;
}

