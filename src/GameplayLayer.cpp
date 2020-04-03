#include "GameplayLayer.h"

GameplayLayer::GameplayLayer()
{
	Aegis::Application::SetVsync(true);

	levels_.emplace(Level("assets/level/level1.txt", player_));
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

	for (auto it = levels_.top().game_objects_.begin(); it != levels_.top().game_objects_.end();) {
		if (HasCollided(player_, **it)) {
			if ((**it).destructible_) {
				it = levels_.top().game_objects_.erase(it);
				continue;
			}
			else {
				ResolveCollision(player_, **it);
				++it;
				continue;
			}
		}
		++it;
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
	levels_.top().OnRender(delta_time);
	Aegis::Renderer2D::DrawText(std::to_string(Aegis::Application::GetFrameTime()), { 0, 0 }, { 1.0f, 1.0f, 1.0f, 1.0f });

}