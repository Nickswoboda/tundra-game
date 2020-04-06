#include "GameplayLayer.h"

GameplayLayer::GameplayLayer()
	:player_(Player(20, 20))
{
	Aegis::Application::SetVsync(true);

	levels_.emplace(Level("assets/levels/level2.txt", player_));
	levels_.emplace(Level("assets/levels/level1.txt", player_));
}

bool GameplayLayer::HasCollided(const GameObject& obj_1, const glm::vec2& tile_index)
{
	auto& tile_map = levels_.top().tile_map_;
	if ((int)obj_1.x_pos_ < tile_index.x * tile_map.tile_size_ + tile_map.tile_size_ &&
		(int)obj_1.x_pos_ + obj_1.width_ > tile_index.x * tile_map.tile_size_ &&
		(int)obj_1.y_pos_ < tile_index.y * tile_map.tile_size_ + tile_map.tile_size_ &&
		(int)obj_1.y_pos_ + obj_1.height_ > tile_index.y * tile_map.tile_size_) {

		std::cout << "Collision Detected\n";
		return true;
	}

	return false;
}

void GameplayLayer::ResolveCollision(GameObject& obj_1, const glm::vec2& tile_index)
{
	auto& tile_map = levels_.top().tile_map_;
	if (obj_1.x_vel_ > 0) {
		obj_1.x_pos_ = tile_index.x * tile_map.tile_size_ - obj_1.width_;
	}
	else if (obj_1.x_vel_ < 0) {
		obj_1.x_pos_ = tile_index.x * tile_map.tile_size_ + tile_map.tile_size_;
	}
	else if (obj_1.y_vel_ > 0) {
		obj_1.y_pos_ = tile_index.y * tile_map.tile_size_ - obj_1.height_;
	}
	else if (obj_1.y_vel_ < 0) {
		obj_1.y_pos_ = tile_index.y * tile_map.tile_size_ + tile_map.tile_size_;
	}
	obj_1.x_vel_ = 0;
	obj_1.y_vel_ = 0;

}
void GameplayLayer::OnUpdate()
{
	player_.Update();
	
	auto& tile_map = levels_.top().tile_map_;
	for (int i = 0; i < tile_map.height_; ++i) {
		for (int j = 0; j < tile_map.width_; ++j) {
			if (tile_map.tiles_[i][j].is_solid_) {
				if (HasCollided(player_, { j, i })) {
					ResolveCollision(player_, { j, i });
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
	levels_.top().Render(delta_time);
	player_.Render(delta_time);
	Aegis::Renderer2D::DrawText(std::to_string(Aegis::Application::GetFrameTime()), { 0, 0 }, { 1.0f, 1.0f, 1.0f, 1.0f });

}