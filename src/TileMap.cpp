#include "TileMap.h"

#include <fstream>
#include <sstream>

#include <algorithm>
TileMap::TileMap(const std::string& file_path, int tile_size)
	: tile_size_(tile_size)
{
	std::ifstream file(file_path);
	std::stringstream buffer;
	buffer << file.rdbuf();

	int x = 0;
	int y = 0;
	tiles_.push_back(std::vector<Tile>());


	for (const auto& ch : buffer.str()) {
		switch (ch)
		{
			case '\n': {
				tiles_.push_back(std::vector<Tile>());
				++y; x = 0; break;
			}
			case '0': {
				tiles_[y].emplace_back(Wall(x * tile_size, y * tile_size)); ++x; break;
			}
			case '1': {
				tiles_[y].emplace_back(Ice(x * tile_size, y * tile_size));
				pellets_.emplace_back(Pellet(x * tile_size + 6, y * tile_size + 6)); ++x; break;
			}
			case ' ': {
				tiles_[y].emplace_back(Ground(x * tile_size, y * tile_size)); ++x; break;
			}
		}
	}
	height_ = tiles_.size();
	width_ = tiles_[height_ - 1].size();
}

void TileMap::Render()
{
	for (int row = 0; row < tiles_.size(); ++row) {
		for (int col = 0; col < tiles_[row].size(); ++col) {
			
			Aegis::DrawQuad(tiles_[row][col].pos_, Aegis::Vec2(tile_size_, tile_size_), tiles_[row][col].color_);
		}
	}

	for (auto& pellet : pellets_)
	{
		pellet.Render(0.0f);
	}
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
			temp.push_back(&tiles_[j][i]);
		}
	}

	return temp;
}

std::vector<Tile*> TileMap::GetTilesUnderneath(const GameObject& obj)
{
	return GetTilesUnderneath(obj.pos_.x, obj.pos_.y, obj.size_.x, obj.size_.y);
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
