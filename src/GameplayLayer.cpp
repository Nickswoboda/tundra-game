#include "GameplayLayer.h"

GameplayLayer::GameplayLayer()
	:player_(32, 32), tile_map_("assets/levels/level1.txt", 32)
{}

void GameplayLayer::ResolveCollision(GameObject& obj_1, const Tile& tile)
{
	if (obj_1.vel_.x > 0) {
		obj_1.pos_.x = tile.pos_.x - obj_1.size_.x;
	}
	else if (obj_1.vel_.x < 0) {
		obj_1.pos_.x = tile.pos_.x + tile_map_.tile_size_;
	}
	else if (obj_1.vel_.y > 0) {
		obj_1.pos_.y = tile.pos_.y - obj_1.size_.y;
	}
	else if (obj_1.vel_.y < 0) {
		obj_1.pos_.y = tile.pos_.y + tile_map_.tile_size_;
	}
	obj_1.vel_.x = 0;
	obj_1.vel_.y = 0;

}
void GameplayLayer::OnUpdate()
{
	player_.Update();
	
	auto tiles = tile_map_.GetTilesUnderneath(player_);

	for (const auto& tile : tiles) {
		if (tile->is_solid_) {
			ResolveCollision(player_, *tile);
		}
	}
}
void GameplayLayer::OnEvent(Aegis::Event& event)
{
	auto click_event = dynamic_cast<Aegis::MouseClickEvent*>(&event);

	if (click_event && click_event->action_ == GLFW_PRESS) {
		auto mouse_pos = Aegis::Application::GetMousePos();
		auto tile = tile_map_.GetTilesUnderneath(mouse_pos.x, mouse_pos.y, 1, 1);


		if (tile.size() > 0) {
			auto single_tile = tile[0];
			auto index = tile_map_.GetTileIndex(*single_tile);

			tile_map_.tiles_[index.y][index.x] = Wall(index.x * tile_map_.tile_size_, index.y * tile_map_.tile_size_);
		}
	}

	player_.OnEvent(event);
}

void GameplayLayer::OnRender(float delta_time)
{
	Aegis::RendererClear();
	tile_map_.Render();
	player_.Render(delta_time);
	Aegis::DrawText(std::to_string(Aegis::Application::GetFrameTime()), { 0, 0 }, { 1.0f, 1.0f, 1.0f, 1.0f });
}