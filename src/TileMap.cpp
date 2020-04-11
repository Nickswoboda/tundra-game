#include "TileMap.h"

#include <fstream>
#include <sstream>

#include <algorithm>
TileMap::TileMap(const std::string& file_path, int tile_size)
	: tile_size_(tile_size), tile_atlas_("assets/textures/tundra-tile-map.png")
{
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
			case '0': {
				tiles_[col].emplace_back(Wall(col * tile_size, row * tile_size)); ++col; break;
			}
			case '1': {
				tiles_[col].emplace_back(Ice(col * tile_size, row * tile_size));
				pellets_.emplace_back(Pellet(col * tile_size + 6, row * tile_size + 6));  ++col; break;
			}
			case ' ': {
				tiles_[col].emplace_back(Ground(col * tile_size, row * tile_size));  ++col; break;
			}
		}
	}
	width_ = tiles_.size();
	height_ = tiles_[width_ - 1].size();
}

void TileMap::Render()
{
	for (int col = 0; col < tiles_.size(); ++col) {
		for (int row = 0; row < tiles_[col].size(); ++row) {
			Aegis::DrawQuad(tiles_[col][row].pos_, Aegis::Vec2(tile_size_, tile_size_), std::make_unique<Aegis::Texture>(tile_atlas_), { 1.0f, 1.0f, 1.0f, 1.0f }, tiles_[col][row].uv_coords_);
		}
	}

	for (auto& pellet : pellets_)
	{
		pellet.Render(0.0f);
	}
}

Tile* TileMap::GetTileByIndex(int col, int row)
{
	if (row >= height_ || row < 0 || col >= width_ || col < 0) {
		return nullptr;
	}

	return &tiles_[col][row];
}

Tile* TileMap::GetTileByPos(int x_pos, int y_pos)
{
	return GetTileByIndex(x_pos / tile_size_, y_pos / tile_size_);
}

std::vector<Tile*> TileMap::GetTilesUnderneath(int x, int y, int w, int h)
{
	std::vector<Tile*> temp;

	int left_index = std::max(0, x / tile_size_);
	int right_index = std::min(width_, (x + w - 1) / tile_size_);
	int top_index = std::max(0, y / tile_size_);
	int bottom_index = std::min(height_, (y + h - 1) / tile_size_);


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

Aegis::Vec2 TileMap::GetTileIndex(const Tile& tile)
{
	int col = tile.pos_.x / tile_size_;
	int row = tile.pos_.y / tile_size_;

	return Aegis::Vec2(col, row);
}

Aegis::Vec2 TileMap::GetGridIndexByPos(int x, int y)
{
	int col = x / tile_size_;
	int row = y / tile_size_;
	return Aegis::Vec2(col, row);
}
