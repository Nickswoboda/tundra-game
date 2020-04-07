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
				tiles_[y].emplace_back(Ice(x * tile_size, y * tile_size)); ++x; break;
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
			Aegis::Vec2 size = { tile_size_, tile_size_ };
			Aegis::DrawQuad(tiles_[row][col].pos_, size, tiles_[row][col].color_);
		}
	}
}

std::vector<Tile*> TileMap::GetTilesUnderneath(const GameObject& obj)
{
	std::vector<Tile*> temp;

	int left_index = obj.pos_.x / tile_size_;
	int right_index = (obj.pos_.x + obj.size_.x) / tile_size_;
	int top_index = obj.pos_.y / tile_size_;
	int bottom_index = (obj.pos_.y + obj.size_.y - 1) / tile_size_;

	for (int i = left_index; i <= right_index; ++i) {
		for (int j = top_index; j <= bottom_index; ++j) {
			temp.push_back(&tiles_[j][i]);
		}
	}

	return temp;
}
