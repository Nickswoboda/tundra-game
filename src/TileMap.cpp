#include "TileMap.h"

#include <fstream>
#include <sstream>

#include <algorithm>
TileMap::TileMap(const std::string& file_path, int tile_size)
	: tile_size_(tile_size, tile_size)
{
	tile_atlas_ = Aegis::TextureManager::Instance().Load("assets/textures/tundra-tile-map.png");

	std::ifstream file(file_path);
	std::stringstream buffer;
	buffer << file.rdbuf();

	int col = 0;
	int row = 0;

	for (const auto& ch : buffer.str()) {


		if ( ch == '\n'){
			++row; col = 0;
		}
		if (tiles_.size() <= col) {
			tiles_.push_back(std::vector<Tile>());
		}
		switch (ch)
		{
			case 'P': {
				player_start_pos_ = Aegis::Vec2( col, row ); break;
			}
			case 'B': {
				bjorne_start_pos_ = Aegis::Vec2(col, row); break;
			}
			case 'C': {
				brutus_start_pos_ = Aegis::Vec2(col, row); break;
			}
			case '0': {
				tiles_[col].emplace_back(Wall(col * tile_size, row * tile_size)); ++col; break;
			}
			case '1': {
				tiles_[col].emplace_back(Ice(col * tile_size, row * tile_size)); ++col; break;
			}
			case ' ': {
				tiles_[col].emplace_back(Ground(col * tile_size, row * tile_size));  ++col; break;
			}
			
		}
	}
	grid_size_.x = tiles_.size();
	grid_size_.y = tiles_[grid_size_.x - 1].size();
}

void TileMap::Render()
{
	for (int col = 0; col < tiles_.size(); ++col) {
		for (int row = 0; row < tiles_[col].size(); ++row) {
			Aegis::DrawQuad(tiles_[col][row].pos_, tile_size_, tile_atlas_, { 1.0f, 1.0f, 1.0f, 1.0f }, tiles_[col][row].uv_coords_);
		}
	}
}

Tile* TileMap::GetTileByIndex(int col, int row)
{
	if (row >= grid_size_.y || row < 0 || col >= grid_size_.x || col < 0) {
		return nullptr;
	}

	return &tiles_[col][row];
}

Tile* TileMap::GetTileByPos(int x_pos, int y_pos)
{
	return GetTileByIndex(x_pos / tile_size_.x, y_pos / tile_size_.y);
}

std::vector<Tile*> TileMap::GetTilesUnderneath(int x, int y, int w, int h)
{
	std::vector<Tile*> temp;

	int left_index = std::max(0.0f, x / tile_size_.x);
	int right_index = std::min(grid_size_.x, (x + w - 1) / tile_size_.x);
	int top_index = std::max(0.0f, y / tile_size_.y);
	int bottom_index = std::min(grid_size_.y, (y + h - 1) / tile_size_.y);


	for (int i = left_index; i <= right_index; ++i) {
		for (int j = top_index; j <= bottom_index; ++j) {
			Tile* tile = GetTileByIndex(i, j);
			if (tile != nullptr) {
				temp.push_back(&tiles_[i][j]);
			}
		}
	}

	return temp;
}

std::vector<Tile*> TileMap::GetTilesUnderneath(const Aegis::AABB& rect)
{
	return GetTilesUnderneath(rect.pos.x, rect.pos.y, rect.size.x, rect.size.y);
}

void TileMap::SetTextureAtlas(const Aegis::Texture& atlas)
{
	tile_atlas_ = std::make_unique<Aegis::Texture>(atlas);
}

Aegis::Vec2 TileMap::GetTileIndex(const Tile& tile)
{
	return tile.pos_ / tile_size_;
}

Aegis::Vec2 TileMap::GetGridIndexByPos(const Aegis::Vec2& pos)
{
	return pos / tile_size_;
}
