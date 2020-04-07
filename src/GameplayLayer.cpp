#include "GameplayLayer.h"

GameplayLayer::GameplayLayer()
	:player_(32, 32), tile_map_("assets/levels/level1.txt", 32)
{
	Aegis::Application::SetVsync(true);
}

bool GameplayLayer::HasCollided(const GameObject& obj_1, int tile_x, int tile_y)
{
	if ((int)obj_1.pos_.x < tile_x * tile_map_.tile_size_ + tile_map_.tile_size_ &&
		(int)obj_1.pos_.x + obj_1.size_.x > tile_x * tile_map_.tile_size_ &&
		(int)obj_1.pos_.y < tile_y * tile_map_.tile_size_ + tile_map_.tile_size_ &&
		(int)obj_1.pos_.y + obj_1.size_.y > tile_y * tile_map_.tile_size_) {

		std::cout << "Collision Detected\n";
		return true;
	}

	return false;
}

void GameplayLayer::ResolveCollision(GameObject& obj_1, int tile_x, int tile_y)
{
	if (obj_1.x_vel_ > 0) {
		obj_1.pos_.x = tile_x * tile_map_.tile_size_ - obj_1.size_.x;
	}
	else if (obj_1.x_vel_ < 0) {
		obj_1.pos_.x = tile_x * tile_map_.tile_size_ + tile_map_.tile_size_;
	}
	else if (obj_1.y_vel_ > 0) {
		obj_1.pos_.y = tile_y * tile_map_.tile_size_ - obj_1.size_.y;
	}
	else if (obj_1.y_vel_ < 0) {
		obj_1.pos_.y = tile_y * tile_map_.tile_size_ + tile_map_.tile_size_;
	}
	obj_1.x_vel_ = 0;
	obj_1.y_vel_ = 0;

}
void GameplayLayer::OnUpdate()
{
	player_.Update();
	
	for (int i = 0; i < tile_map_.height_; ++i) {
		for (int j = 0; j < tile_map_.width_; ++j) {
			if (tile_map_.tiles_[i][j].is_solid_) {
				if (HasCollided(player_, j, i)) {
					ResolveCollision(player_, j, i);
				}
			}
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
	tile_map_.Render();
	player_.Render(delta_time);
	Aegis::Renderer2D::DrawText(std::to_string(Aegis::Application::GetFrameTime()), { 0, 0 }, { 1.0f, 1.0f, 1.0f, 1.0f });

}