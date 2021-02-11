#include "GameObject.h"

static const int TILE_SIZE = 32;

void GameObject::MoveTo(const Aegis::Vec2 pos)
{
	Aegis::Vec2 prev_tile_pos = rect_.pos / TILE_SIZE;
	Aegis::Vec2 vec = pos - prev_tile_pos;
	int num_tiles = sqrt(vec.x * vec.x + vec.y * vec.y);
	target_grid_index_ = pos;

	if (vec.x < 0) {
		sprite_.horizontal_flip = true;
	}
	else if (vec.x > 0){
		sprite_.horizontal_flip = false;
	}

	animation_.Start(rect_.pos, pos * TILE_SIZE, speed_ * num_tiles);
}

void Player::MoveTo(const Aegis::Vec2 pos)
{
	if (!animation_.playing_){
		GameObject::MoveTo(pos);
	}
	else{
		queued_movement_ = pos;
	}
}

void GameObject::Update()
{
	if (animation_.playing_) {
		animation_.Update();
		rect_.pos = animation_.current_value_;

		wiggle_timer_.Update();
		if (wiggle_timer_.stopped_) {
			if (sprite_.rotation_ == -22.5f || sprite_.rotation_ == 22.5f) {
				sprite_.rotation_ = 0.0f;
			}
			else {
				sprite_.rotation_ = sprite_.horizontal_flip ? 22.5f : -22.5f;
			}
			wiggle_timer_.Start(speed_ * 1000.0);
		}

		if (rect_.pos == target_grid_index_ * TILE_SIZE) {
			animation_.Stop();
			sprite_.rotation_ = 0;
			grid_index_ = target_grid_index_;
		}
	}
}

void Player::Update()
{
	if (animation_.playing_) {
		animation_.Update();
		rect_.pos = animation_.current_value_;
		wiggle_timer_.Update();
		if (wiggle_timer_.stopped_) {
			if (sprite_.rotation_ == -22.5f || sprite_.rotation_ == 22.5f) {
				sprite_.rotation_ = 0.0f;
			}
			else {
				sprite_.rotation_ = sprite_.horizontal_flip ? 22.5f : -22.5f;
			}
			wiggle_timer_.Start(speed_ * 1000.0);
		}

		if (rect_.pos == target_grid_index_ * TILE_SIZE) {
			animation_.Stop();
			sprite_.rotation_ = 0;
			grid_index_ = target_grid_index_;

			if (queued_movement_.x != -1) {
				MoveTo(queued_movement_);
				queued_movement_.x = -1;
			}
		}
	}
}

void GameObject::SetPosition(Aegis::Vec2 pos)
{
	rect_.pos = pos;
	grid_index_ = pos / TILE_SIZE;
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

