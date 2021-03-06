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

void GameObject::Update()
{
	animation_.Update();
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
	animation_.Stop();
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
	if (!playing_) return;

	if (current_frame_ == total_frames_) {
		sprite_.position_ = end_value_;
		Stop();
	}
	else {
		float percentage = current_frame_ / (float)total_frames_;
		++current_frame_;
		current_value_ = Aegis::LERP(start_value_, end_value_, percentage);
		float eps = 0.05f;
		sprite_.position_ = Aegis::Vec2(static_cast<int>(current_value_.x + eps), static_cast<int>(current_value_.y + eps));

		int rotations = percentage / percent_to_rotate_at_;
		if (rotations > current_rotations_) {
			sprite_.rotation_ = sprite_.rotation_ == 0 ? (sprite_.GetHorizontalFlip() ? 22.5f : -22.5f) : 0;
			++current_rotations_;
			if (sfx_ != -1) {
				Aegis::AudioPlayer::PlaySound(sfx_);
			}
		}
	}
}

void Animation::Stop()
{
	playing_ = false;
	current_frame_ = 0;
	sprite_.rotation_ = 0;
}
