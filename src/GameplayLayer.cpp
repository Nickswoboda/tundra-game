#include "GameplayLayer.h"

GameplayLayer::GameplayLayer()
{
	Aegis::Application::SetVsync(true);
	for (int i = 0; i < 20; ++i) {
		walls_[i].x_pos_ = i * 32;
	}
	for (int i = 20; i < 40; ++i) {
		walls_[i].x_pos_ = (i - 20) * 32;
		walls_[i].y_pos_ = 480 - 32;
	}
	for (int i = 40; i < 60; ++i) {
		walls_[i].x_pos_ = 640 - 32;
		walls_[i].y_pos_ = (i - 40) * 32;
	}
	for (int i = 60; i < 80; ++i) {
		walls_[i].y_pos_ = (i - 60) * 32;
	}

	pellets_.emplace_back(new Pellet());
	pellets_[0]->x_pos_ = 100;
	pellets_[0]->y_pos_ = 40;

	player_.x_pos_ = 40;
	player_.y_pos_ = 40;

}

bool GameplayLayer::HasCollided(const GameObject& obj_1, const GameObject& obj_2)
{
	if ((int)obj_1.x_pos_ < obj_2.x_pos_ + obj_2.width_ &&
		(int)obj_1.x_pos_ + obj_1.width_ > obj_2.x_pos_ &&
		(int)obj_1.y_pos_ < obj_2.y_pos_ + obj_2.height_ &&
		(int)obj_1.y_pos_ + obj_1.height_ > obj_2.y_pos_) {

		std::cout << "Collision Detected\n";
		return true;
	}

	return false;
}

void GameplayLayer::ResolveCollision(GameObject& obj_1, GameObject& obj_2) {
	if (obj_1.x_vel_ > 0) {
		int x_overlap = (int)obj_1.x_pos_ + obj_1.width_ - obj_2.x_pos_;
		obj_1.x_pos_ -= x_overlap;
	}
	else if (obj_1.x_vel_ < 0) {
		int x_overlap = obj_2.x_pos_ + obj_2.width_ - (int)obj_1.x_pos_;
		obj_1.x_pos_ += x_overlap;
	}

	if (obj_1.y_vel_ > 0) {
		int y_overlap = (int)obj_1.y_pos_ + obj_1.height_ - obj_2.y_pos_;
		obj_1.y_pos_ -= y_overlap;
	}
	else if (obj_1.y_vel_ < 0) {
		int y_overlap = obj_2.y_pos_ + obj_2.height_ - (int)obj_1.y_pos_;
		obj_1.y_pos_ += y_overlap;
	}

	obj_1.x_vel_ = 0;
	obj_1.y_vel_ = 0;
}
void GameplayLayer::OnUpdate()
{
	player_.OnUpdate();

	for (auto& wall : walls_) {
		if (HasCollided(player_, wall)) {
			ResolveCollision(player_, wall);
		}
	}

	for (auto i = pellets_.begin(); i != pellets_.end();) {
		if (HasCollided(player_, **i)) {
			i = pellets_.erase(i);
		}
		else {
			++i;
		}

	}
}
void GameplayLayer::OnEvent(Aegis::Event& event)
{
	player_.OnEvent(event);
}

void GameplayLayer::OnRender(float delta_time)
{
	Aegis::Renderer2D::Clear();
	player_.OnRender(delta_time);
	for (const auto& wall : walls_) {
		wall.OnRender(delta_time);
	}
	for (const auto& pellet : pellets_) {
		pellet->OnRender(delta_time);
	}
	Aegis::Renderer2D::DrawText(std::to_string(Aegis::Application::GetFrameTime()), { 0, 0 }, { 1.0f, 1.0f, 1.0f, 1.0f });

}