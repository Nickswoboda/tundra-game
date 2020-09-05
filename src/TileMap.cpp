#include "TileMap.h"

#include <filesystem>
#include <fstream>
#include <sstream>

#include <algorithm>
TileMap::TileMap(const std::string& file_path, int tile_size, std::shared_ptr<Aegis::Texture> atlas)
	: tile_atlas_(atlas), tile_size_(tile_size),
	  wall_tile_(std::make_shared<Aegis::SubTexture>(tile_atlas_, Aegis::Vec2(0.0f, 0.0f), Aegis::Vec2(32.0f, 32.0f)), true, false),
	  ice_tile_(std::make_shared<Aegis::SubTexture>(tile_atlas_, Aegis::Vec2(64.0f, 0.0f), Aegis::Vec2(32.0f, 32.0f)), false, true),
	  ground_tile_(std::make_shared<Aegis::SubTexture>(tile_atlas_, Aegis::Vec2(32.0f, 0.0f), Aegis::Vec2(32.0f, 32.0f)), false, false)
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
			tiles_.push_back(std::vector<Tile*>());
		}
		switch (ch)
		{
			case 'P': {
				bruce_spawn_index_ = Aegis::Vec2( col, row ); break;
			}
			case 'B': {
				bjorn_spawn_index_ = Aegis::Vec2(col, row); break;
			}
			case 'C': {
				brutus_spawn_index_ = Aegis::Vec2(col, row); break;
			}
			case '0': {
				tiles_[col].push_back(&wall_tile_); ++col; break;
			}
			case '1': {
				tiles_[col].push_back(&ice_tile_); ++col; break;
			}
			case ' ': {
				tiles_[col].push_back(&ground_tile_); ++col; break;
			}
			
		}
	}
	grid_size_.x = tiles_.size();
	grid_size_.y = tiles_[grid_size_.x - 1].size();
}

TileMap::TileMap(int width, int height, int tile_size, std::shared_ptr<Aegis::Texture> atlas)
	: tile_atlas_(atlas), tile_size_(tile_size),
	  wall_tile_(std::make_shared<Aegis::SubTexture>(tile_atlas_, Aegis::Vec2(0.0f, 0.0f), Aegis::Vec2(32.0f, 32.0f)), true, false),
	  ice_tile_(std::make_shared<Aegis::SubTexture>(tile_atlas_, Aegis::Vec2(64.0f, 0.0f), Aegis::Vec2(32.0f, 32.0f)), false, true),
	  ground_tile_(std::make_shared<Aegis::SubTexture>(tile_atlas_, Aegis::Vec2(32.0f, 0.0f), Aegis::Vec2(32.0f, 32.0f)), false, false)
{
	brutus_spawn_index_ = {1, 0};
	bjorn_spawn_index_ = {2, 0};
	for (int i = 0; i < width; ++i){
		std::vector<Tile*> col;
		for (int j = 0; j < height; ++j){
			col.push_back(&wall_tile_);
		}
		tiles_.push_back(col);
	}
	grid_size_.x = width;
	grid_size_.y = height;
	

}

void TileMap::Render() const
{
	for (int col = 0; col < tiles_.size(); ++col) {
		for (int row = 0; row < tiles_[col].size(); ++row) {
			Aegis::DrawQuad({col * 32.0f, row * 32.0f}, tiles_[col][row]->texture_);
		}
	}
}

const Tile& TileMap::GetTileByIndex(int col, int row)
{
	if (row >= grid_size_.y || row < 0 || col >= grid_size_.x || col < 0) {
		return nullptr;
	}

	return tiles_[col][row];
}

Tile* TileMap::GetTileByPos(int x_pos, int y_pos)
{
	return GetTileByIndex(x_pos / tile_size_, y_pos / tile_size_);
}

std::vector<Aegis::Vec2> TileMap::GetAdjacentTilesIndices(Aegis::Vec2 index)
{
	std::vector<Aegis::Vec2> indices;
	
	Aegis::Vec2 up = {index.x, index.y - 1};
	if (GetTileByIndex(up.x, up.y)) indices.push_back(up);

	Aegis::Vec2 down = {index.x, index.y + 1};
	if (GetTileByIndex(down.x, down.y)) indices.push_back(down);

	Aegis::Vec2 left = {index.x - 1, index.y};
	if (GetTileByIndex(left.x, left.y)) indices.push_back(left);

	Aegis::Vec2 right = {index.x + 1, index.y};
	if (GetTileByIndex(right.x, right.y)) indices.push_back(right);
	
	return indices;
}

std::vector<Tile*> TileMap::GetTilesUnderneath(int x, int y, int w, int h)
{
	std::vector<Tile*> temp;

	int left_index = std::max(0.0f, (float)x / tile_size_);
	int right_index = std::min(grid_size_.x, (float)(x + w - 1) / tile_size_);
	int top_index = std::max(0.0f, (float)y / tile_size_);
	int bottom_index = std::min(grid_size_.y,(float) (y + h - 1) / tile_size_);


	for (int i = left_index; i <= right_index; ++i) {
		for (int j = top_index; j <= bottom_index; ++j) {
			Tile* tile = GetTileByIndex(i, j);
			if (tile != nullptr) {
				temp.push_back(tiles_[i][j]);
			}
		}
	}

	return temp;
}

std::vector<Tile*> TileMap::GetTilesUnderneath(const Aegis::AABB& rect)
{
	return GetTilesUnderneath(rect.pos.x, rect.pos.y, rect.size.x, rect.size.y);
}

std::vector<std::vector<bool>> TileMap::GetReachableTileIndices(Aegis::Vec2 start_index)
{
	std::vector<Aegis::Vec2> frontier;
	frontier.push_back(start_index);
	std::vector<std::vector<bool>> seen(grid_size_.x, std::vector<bool>(grid_size_.y, false));
	seen[start_index.x][start_index.y] = true;

	while (!frontier.empty()) {
		auto current_pos = frontier[0];
		frontier.erase(frontier.begin());

		auto neighbors = GetAdjacentTilesIndices(current_pos);
		
		for (auto index : neighbors){
			if (tiles_[index.x][index.y]->is_solid_){
				if (!seen[index.x][index.y]){
					frontier.push_back(index);
					seen[index.x][index.y] = true;
				}
			}
		}
	}
	return seen;
}

Aegis::Vec2 TileMap::GetGridIndexByPos(const Aegis::Vec2& pos) const
{
	return pos / tile_size_;
}

void TileMap::Save()
{
	int level = 1;
	std::string new_file_path = "assets/levels/level" + std::to_string(level) + ".txt";

	while (std::filesystem::exists(new_file_path)) {
		++level;
		new_file_path = "assets/levels/level" + std::to_string(level) + ".txt";
	}
	std::ofstream file(new_file_path);

	for (int row = 0; row < grid_size_.y; ++row) {
		for (int col = 0; col < grid_size_.x; ++col) {
			auto coord = Aegis::Vec2(col, row);

			if (coord == brutus_spawn_index_) {
				file << 'C';
			}
			else if (coord == bruce_spawn_index_) {
				file << 'P';
			}
			else if (coord == bjorn_spawn_index_) {
				file << 'B';
			}

			auto tile = tiles_[col][row];
			if (tile->is_solid_) file << '0';
			else if (tile->is_slippery_) file << '1';
			else file << ' ';
		}

		//don't print newline on last row
		if (row != grid_size_.y - 1) {
			file << '\n';
		}
	}
	file.close();
}
