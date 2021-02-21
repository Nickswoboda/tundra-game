#include "TileMap.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>

#include <algorithm>
TileMap::TileMap(const std::string& file_path, int tile_size, std::shared_ptr<Aegis::Texture> atlas)
	: tile_atlas_(atlas), tile_size_(tile_size)
{
	LoadTiles();
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
			tiles_.push_back(std::vector<const Tile*>());
		}

		if (ch == 's' || ch =='j' || ch == 'p' || ch == 'f'){
			if (ch == 's'){
				bruce_spawn_index_ = Aegis::Vec2( col, row ); 
			} else if (ch == 'j'){
				bjorn_spawn_index_ = Aegis::Vec2(col, row);
			} else if (ch == 'p'){
				brutus_spawn_index_ = Aegis::Vec2(col, row);
			} else {
				pellet_spawn_indices_.emplace_back(col, row);
			}

			tiles_[col++].push_back(&tiles_map_['i']);
		} else if (tiles_map_.count(ch)){
			tiles_[col++].push_back(&tiles_map_[ch]);
		} else if (ch != '\n'){
			std::cout << ch << " is not a recognized tile symbol. File: " << file_path << "\n";
		}
	}

	grid_size_.x = tiles_.size();
	grid_size_.y = tiles_[grid_size_.x - 1].size();
}

TileMap::TileMap(int width, int height, int tile_size, std::shared_ptr<Aegis::Texture> atlas)
	: tile_atlas_(atlas), tile_size_(tile_size)
{
	LoadTiles();
	brutus_spawn_index_ = {1, 0};
	bjorn_spawn_index_ = {2, 0};
	for (int i = 0; i < width; ++i){
		std::vector<const Tile*> col;
		for (int j = 0; j < height; ++j){
			col.push_back(&tiles_map_['w']);
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
			Aegis::DrawSubTexture({col * 32.0f, row * 32.0f}, tiles_[col][row]->size_, *tiles_[col][row]->texture_, tiles_[col][row]->texture_coords_);
		}
	}
}

const Tile* TileMap::GetTileByIndex(int col, int row)
{
	if (row >= grid_size_.y || row < 0 || col >= grid_size_.x || col < 0) {
		return nullptr;
	}

	return tiles_[col][row];
}

const Tile* TileMap::GetTileByPos(int x_pos, int y_pos)
{
	return GetTileByIndex(x_pos / tile_size_, y_pos / tile_size_);
}

void TileMap::SetTile(const Aegis::Vec2 index, const Tile& tile)
{
	tiles_[index.x][index.y] = &tile;
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

std::vector<const Tile*> TileMap::GetTilesUnderneath(int x, int y, int w, int h)
{
	std::vector<const Tile*> temp;

	int left_index = std::max(0.0f, (float)x / tile_size_);
	int right_index = std::min(grid_size_.x, (float)(x + w - 1) / tile_size_);
	int top_index = std::max(0.0f, (float)y / tile_size_);
	int bottom_index = std::min(grid_size_.y,(float) (y + h - 1) / tile_size_);


	for (int i = left_index; i <= right_index; ++i) {
		for (int j = top_index; j <= bottom_index; ++j) {
			const Tile* tile = GetTileByIndex(i, j);
			if (tile != nullptr) {
				temp.push_back(tiles_[i][j]);
			}
		}
	}

	return temp;
}

std::vector<const Tile*> TileMap::GetTilesUnderneath(const Aegis::AABB& rect)
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
			if (!tiles_[index.x][index.y]->is_solid_){
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
	auto index = pos / tile_size_;
	//want to truncate for positioning
	return Aegis::Vec2((int)index.x, (int)index.y);
}

void TileMap::Save(int level_num)
{
	std::string new_file_path = "assets/levels/level_" + std::to_string(level_num) + ".txt";

	if (level_num == -1){
		level_num = 1;
		new_file_path = "assets/levels/level_" + std::to_string(level_num) + ".txt";

		while (std::filesystem::exists(new_file_path)) {
			++level_num;
			new_file_path = "assets/levels/level_" + std::to_string(level_num) + ".txt";
		}
	}

	std::ofstream file(new_file_path);

	for (int row = 0; row < grid_size_.y; ++row) {
		for (int col = 0; col < grid_size_.x; ++col) {
			auto coord = Aegis::Vec2(col, row);

			if (coord == brutus_spawn_index_) {
				file << 'p';
			}
			else if (coord == bruce_spawn_index_) {
				file << 's';
			}
			else if (coord == bjorn_spawn_index_) {
				file << 'j';
			}
			else {
				auto tile = tiles_[col][row];
				if (tile->is_solid_) file << 'w';
				else if (tile->is_slippery_) file << 'i';
				else file << 'g';
			}
		}

		//don't print newline on last row
		if (row != grid_size_.y - 1) {
			file << '\n';
		}
	}
	file.close();
}

void TileMap::LoadTiles()
{
	tiles_map_.emplace('0', Tile(tile_atlas_, {0, 0, 32, 32}, true, false));
	tiles_map_.emplace('1', Tile(tile_atlas_, {32, 0, 32, 32}, true, false));
	tiles_map_.emplace('2', Tile(tile_atlas_, {64, 0, 32, 32}, true, false));
	tiles_map_.emplace('3', Tile(tile_atlas_, {0, 32, 32, 32}, true, false));
	tiles_map_.emplace('4', Tile(tile_atlas_, {64, 32, 32, 32}, true, false));
	tiles_map_.emplace('5', Tile(tile_atlas_, {0, 64, 32, 32}, true, false));
	tiles_map_.emplace('6', Tile(tile_atlas_, {32, 64, 32, 32}, true, false));
	tiles_map_.emplace('7', Tile(tile_atlas_, {64, 64, 32, 32}, true, false));
	tiles_map_.emplace('8', Tile(tile_atlas_, { 96, 0, 32, 32 }, true, false));
	tiles_map_.emplace('9', Tile(tile_atlas_, { 128, 0, 32, 32 }, true, false));
	tiles_map_.emplace('a', Tile(tile_atlas_, { 96, 32, 32, 32 }, true, false));
	tiles_map_.emplace('b', Tile(tile_atlas_, { 128, 32, 32, 32 }, true, false));
	tiles_map_.emplace('i', Tile(tile_atlas_, {32, 32, 32, 32}, false, true));
	tiles_map_.emplace('w', Tile(tile_atlas_, {96, 64, 32, 32}, true, false));
	tiles_map_.emplace('g', Tile(tile_atlas_, {128, 64, 32, 32}, false, false));
}

void TileMap::Clear()
{
	for (auto& row : tiles_)
	{
		for (auto& tile : row) {
			tile = &tiles_map_['w'];
		}
	}

	bruce_spawn_index_ = { 0, 0 };
	brutus_spawn_index_ = { 1, 0 };
	bjorn_spawn_index_ = { 2, 0 };
} 
