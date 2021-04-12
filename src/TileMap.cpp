#include "TileMap.h"

#include "Utilities.h"

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
				spawn_indices_[SpawnPoint::Bruce] = Aegis::Vec2( col, row ); 
			} else if (ch == 'j'){
				spawn_indices_[SpawnPoint::Bjorn] = Aegis::Vec2( col, row ); 
			} else if (ch == 'p'){
				spawn_indices_[SpawnPoint::Brutus] = Aegis::Vec2( col, row ); 
			} else {
				pellet_spawn_indices_.emplace(col, row);
			}

			tiles_[col++].push_back(&tiles_by_token['i']);
		} else if (tiles_by_token.count(ch)){
			tiles_[col++].push_back(&tiles_by_token[ch]);
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
	spawn_indices_[SpawnPoint::Brutus] = { 1, 0 };
	spawn_indices_[SpawnPoint::Bjorn] = { 2, 0 };
	for (int i = 0; i < width; ++i){
		std::vector<const Tile*> col;
		for (int j = 0; j < height; ++j){
			col.push_back(&tiles_by_token['w']);
		}
		tiles_.push_back(col);
	}
	grid_size_.x = width;
	grid_size_.y = height;
}

void TileMap::Render() const
{
	DrawFrameBorder({{0,0}, grid_size_ * tile_size_});
	for (int col = 0; col < tiles_.size(); ++col) {
		for (int row = 0; row < tiles_[col].size(); ++row) {
			Aegis::DrawSubTexture({col * 32.0f, row * 32.0f}, tiles_[col][row]->size_, *tiles_[col][row]->texture_, tiles_[col][row]->texture_coords_);
		}
	}
}

const Tile* TileMap::GetTileByIndex(Aegis::Vec2 index) const
{
	if (index.y >= grid_size_.y || index.y < 0 || index.x >= grid_size_.x || index.x < 0) {
		return nullptr;
	}

	return tiles_[index.x][index.y];
}

const Tile* TileMap::GetTileByPos(Aegis::Vec2 pos) const
{
	return GetTileByIndex(pos / tile_size_);
}

void TileMap::SetTile(const Aegis::Vec2 index, const char token)
{
	tiles_[index.x][index.y] = &tiles_by_token[token];
}
void TileMap::SetTile(const Aegis::Vec2 index, const Tile& tile)
{
	tiles_[index.x][index.y] = &tile;
}

std::vector<Aegis::Vec2> TileMap::GetAdjacentTilesIndices(Aegis::Vec2 index) const 
{
	std::vector<Aegis::Vec2> indices;
	
	Aegis::Vec2 up = {index.x, index.y - 1};
	if (GetTileByIndex(up)) indices.push_back(up);

	Aegis::Vec2 down = {index.x, index.y + 1};
	if (GetTileByIndex(down)) indices.push_back(down);

	Aegis::Vec2 left = {index.x - 1, index.y};
	if (GetTileByIndex(left)) indices.push_back(left);

	Aegis::Vec2 right = {index.x + 1, index.y};
	if (GetTileByIndex(right)) indices.push_back(right);
	
	return indices;
}

std::vector<std::vector<bool>> TileMap::GetReachableTileIndices(Aegis::Vec2 start_index) const
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
	
	if (index.y >= grid_size_.y || index.y < 0 || index.x >= grid_size_.x || index.x < 0) {
		return {-1, -1};
	}

	return Aegis::Vec2((int)index.x, (int)index.y);
}

void TileMap::Save(int level_num)
{
	std::string prefix = "assets/levels/custom_level_";
	std::string new_file_path = prefix + std::to_string(level_num) + ".txt";

	if (level_num == -1){
		level_num = 1;
		new_file_path = prefix + std::to_string(level_num) + ".txt";

		while (std::filesystem::exists(new_file_path)) {
			++level_num;
			new_file_path = prefix + std::to_string(level_num) + ".txt";
		}
	}

	std::ofstream file(new_file_path);

	for (int row = 0; row < grid_size_.y; ++row) {
		for (int col = 0; col < grid_size_.x; ++col) {
			auto coord = Aegis::Vec2(col, row);

			for (const auto& [spawn, index] : spawn_indices_){
				if (coord == index){
					switch (spawn){
						case SpawnPoint::Brutus: file << 'p'; break;
						case SpawnPoint::Bruce: file << 'p'; break;
						case SpawnPoint::Bjorn: file << 'p'; break;
					}
					continue;
				}
			}
			if (pellet_spawn_indices_.count(coord)){
				file << 'f';
			} else {
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
	tiles_by_token.emplace('0', Tile(tile_atlas_, {0, 0, 32, 32}, true, false));
	tiles_by_token.emplace('1', Tile(tile_atlas_, {32, 0, 32, 32}, true, false));
	tiles_by_token.emplace('2', Tile(tile_atlas_, {64, 0, 32, 32}, true, false));
	tiles_by_token.emplace('3', Tile(tile_atlas_, {0, 32, 32, 32}, true, false));
	tiles_by_token.emplace('4', Tile(tile_atlas_, {64, 32, 32, 32}, true, false));
	tiles_by_token.emplace('5', Tile(tile_atlas_, {0, 64, 32, 32}, true, false));
	tiles_by_token.emplace('6', Tile(tile_atlas_, {32, 64, 32, 32}, true, false));
	tiles_by_token.emplace('7', Tile(tile_atlas_, {64, 64, 32, 32}, true, false));
	tiles_by_token.emplace('8', Tile(tile_atlas_, { 96, 0, 32, 32 }, true, false));
	tiles_by_token.emplace('9', Tile(tile_atlas_, { 128, 0, 32, 32 }, true, false));
	tiles_by_token.emplace('a', Tile(tile_atlas_, { 96, 32, 32, 32 }, true, false));
	tiles_by_token.emplace('b', Tile(tile_atlas_, { 128, 32, 32, 32 }, true, false));
	tiles_by_token.emplace('i', Tile(tile_atlas_, {32, 32, 32, 32}, false, true));
	tiles_by_token.emplace('w', Tile(tile_atlas_, {96, 64, 32, 32}, true, false));
	tiles_by_token.emplace('g', Tile(tile_atlas_, {128, 64, 32, 32}, false, false));
}

void TileMap::Clear()
{
	for (auto& row : tiles_)
	{
		for (auto& tile : row) {
			tile = &tiles_by_token['w'];
		}
	}

	spawn_indices_[SpawnPoint::Bruce] = { 0, 0 };
	spawn_indices_[SpawnPoint::Brutus] = { 1, 0 };
	spawn_indices_[SpawnPoint::Bjorn] = { 2, 0 };

	pellet_spawn_indices_.clear();
} 
