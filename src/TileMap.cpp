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
			
			Aegis::Vec4 tex_coords;
			tex_coords.x = tiles_[row][col].uv_coords_.x / (float) tile_atlas_.width_;
			tex_coords.y = tiles_[row][col].uv_coords_.y / (float) tile_atlas_.height_;
			tex_coords.z = tiles_[row][col].uv_coords_.z / (float) tile_atlas_.width_;
			tex_coords.w = tiles_[row][col].uv_coords_.w / (float) tile_atlas_.height_;

			//Aegis::DrawQuad(tiles_[row][col].pos_, Aegis::Vec2(tile_size_, tile_size_), tiles_[row][col].color_);
			Aegis::DrawQuad(tiles_[row][col].pos_, Aegis::Vec2(tile_size_, tile_size_), std::make_unique<Aegis::Texture>(tile_atlas_), { 1.0f, 1.0f, 1.0f, 1.0f }, tex_coords);
		}
	}

	for (auto& pellet : pellets_)
	{
		pellet.Render(0.0f);
	}
}

Tile* TileMap::GetTileByIndex(int col, int row)
{
	if (col >= height_ || col < 0 || row >= width_ || row < 0) {
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
			Tile* tile = GetTileByIndex(j, i);
			if (tile != nullptr) {
				temp.push_back(&tiles_[j][i]);
			}
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
