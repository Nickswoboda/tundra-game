#include "GameObject.h"

static const int TILE_SIZE = 32;

void GameObject::MoveTo(const Aegis::Vec2 pos)
{
	Aegis::Vec2 prev_tile_pos = sprite_.position_ / TILE_SIZE;
	Aegis::Vec2 vec = pos - prev_tile_pos;
	int num_tiles = sqrt(vec.x * vec.x + vec.y * vec.y);
	target_grid_index_ = pos;

	animation_.Start(pos * TILE_SIZE, speed_, num_tiles);
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

		if (!animation_.playing_){
			grid_index_ = target_grid_index_;
		}
	}
}

void Player::Update()
{
	GameObject::Update();
	if (!animation_.playing_) {
		if (queued_movement_.x != -1) {
			MoveTo(queued_movement_);
			queued_movement_.x = -1;
		}
	}
}

void GameObject::SetPosition(Aegis::Vec2 pos)
{
	sprite_.position_ = pos;
	grid_index_ = pos / TILE_SIZE;
}

Aegis::AABB GameObject::GetRect() const
{
	return sprite_.GetRect();
}

void Animation::Start(Aegis::Vec2 end, float speed, int num_tiles)
{
	start_value_ = sprite_.position_;
	end_value_ = end;

	if ((end_value_ - start_value_).x < 0){
		sprite_.SetHorizontalFlip(true);
	} else if ((end_value_ - start_value_).x > 0){
		sprite_.SetHorizontalFlip(false);
	}

	total_frames_ = (speed*num_tiles) / (1/60.0f);
	percent_to_rotate_at_ = 1.0f / (num_tiles *2);
	current_rotations_ = 0;
	playing_ = true;
}

void Animation::Update()
{
	float percentage = current_frame_ / (float)total_frames_;
	++current_frame_;
	current_value_ = Aegis::LERP(start_value_, end_value_, percentage);
	sprite_.position_ = current_value_;

	if (current_value_ == end_value_) {
		Stop();
	}
	else {
		int rotations = percentage / percent_to_rotate_at_;
		if (rotations > current_rotations_) {
			sprite_.rotation_ = sprite_.rotation_ == 0 ? (sprite_.GetHorizontalFlip() ? 22.5f : -22.5f) : 0;
			++current_rotations_;
			if (sfx_) {
				Aegis::AudioPlayer::Play(*sfx_);
			}
		}
	}
}

void Animation::Stop()
{
	playing_ = false;
	current_frame_ = 0;
	sprite_.rotation_ = 0;
	if (sfx_) {
		sfx_->Stop();
	}
}
