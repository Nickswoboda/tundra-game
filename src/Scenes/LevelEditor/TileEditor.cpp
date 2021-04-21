#include "TileEditor.h"

#include <filesystem>
#include <fstream>

int GetSimplifiedFlags(int flags){
	if (flags & Top){
		flags &= ~TopLeft;
		flags &= ~TopRight;
	}
	if (flags & Bottom){
		flags &= ~BottomLeft;
		flags &= ~BottomRight;
	}
	if (flags & Left){
		flags &= ~TopLeft;
		flags &= ~BottomLeft;
	}
	if (flags & Right){
		flags &= ~TopRight;
		flags &= ~BottomRight;
	}
	
	return flags;
}

TileEditor::TileEditor(TileMap& tile_map)
	:tile_map_(tile_map)
{
	tile_tokens_by_flags_.emplace(None, 'w');
	tile_tokens_by_flags_.emplace(Top, '6');
	tile_tokens_by_flags_.emplace(Bottom, '1');
	tile_tokens_by_flags_.emplace(Left, '4');
	tile_tokens_by_flags_.emplace(Right, '3');
	tile_tokens_by_flags_.emplace(TopLeft, '7');
	tile_tokens_by_flags_.emplace(TopRight, '5');
	tile_tokens_by_flags_.emplace(BottomLeft, '2');
	tile_tokens_by_flags_.emplace(BottomRight, '0');
	tile_tokens_by_flags_.emplace(Top | Left, '8');
	tile_tokens_by_flags_.emplace(Top | Right, '9');
	tile_tokens_by_flags_.emplace(Bottom | Left, 'a');
	tile_tokens_by_flags_.emplace(Bottom | Right, 'b');

	for (int i = 0; i < tile_map.grid_size_.x; ++i){
		ice_flags_.emplace_back((tile_map_.grid_size_.y));
	}

	CalculateIceNeighborFlags();
}

void TileEditor::CalculateIceNeighborFlags()
{
	for (int col = 0; col < ice_flags_.size(); ++col){
		for (int row = 0; row < ice_flags_[0].size(); ++row){
			auto index = Aegis::Vec2(col, row);
			int flags = 0;

			auto top = tile_map_.GetTileByIndex(index + Aegis::Vec2{0, -1});
			if (top && !top->is_solid_){
				flags |= IceNeighborFlags::Top;
			}

			auto bot = tile_map_.GetTileByIndex(index + Aegis::Vec2{0, 1});
			if (bot && !bot->is_solid_){
				flags |= IceNeighborFlags::Bottom;
			}
			auto left = tile_map_.GetTileByIndex(index + Aegis::Vec2{-1, 0});
			if (left && !left->is_solid_){
				flags |= IceNeighborFlags::Left;
			}
			auto right = tile_map_.GetTileByIndex(index + Aegis::Vec2{1, 0});
			if (right && !right->is_solid_){
				flags |= IceNeighborFlags::Right;
			}
			auto top_left = tile_map_.GetTileByIndex(index + Aegis::Vec2{-1, -1});
			if (top_left && !top_left->is_solid_){
				flags |= IceNeighborFlags::TopLeft;
			}
			auto top_right = tile_map_.GetTileByIndex(index + Aegis::Vec2{1, -1});
			if (top_right && !top_right->is_solid_){
				flags |= IceNeighborFlags::TopRight;
			}
			auto bot_left = tile_map_.GetTileByIndex(index + Aegis::Vec2{-1, 1});
			if (bot_left && !bot_left->is_solid_){
				flags |= IceNeighborFlags::BottomLeft;
			}
			auto bot_right = tile_map_.GetTileByIndex(index + Aegis::Vec2{1, 1});
			if (bot_right && !bot_right->is_solid_){
				flags |= IceNeighborFlags::BottomRight;
			}

			ice_flags_[col][row] = flags;
		}
	}
}

void TileEditor::DrawGridLines() const
{
	Aegis::Vec2 line_lengths = tile_map_.grid_size_ * 32;
	Aegis::Vec4 grid_color = { 0.0f, 0.0f, 0.0f, 0.2f };
	for (int i = 0; i <= tile_map_.grid_size_.x; ++i){
		Aegis::DrawQuad({i * 32.0f - 1, 0}, {2, line_lengths.y}, grid_color);
	}

	for (int i = 0; i <= tile_map_.grid_size_.y; ++i){
		Aegis::DrawQuad({0, i * 32.0f - 1}, {line_lengths.x, 2}, grid_color);
	}
}

void TileEditor::SetTile(Aegis::Vec2 index, const Tile& tile)
{
	tile_map_.SetTile(index, tile);
	UpdateSurroundingWallTiles(index);
}

int TileEditor::GetIceNeighborFlags(Aegis::Vec2 index) const
{
	return ice_flags_[index.x][index.y];
}

void TileEditor::UpdateTile(Aegis::Vec2 index)
{
	if (!tile_map_.GetTileByIndex(index)->is_solid_) return;
	
	auto flags = GetSimplifiedFlags(GetIceNeighborFlags(index));
	if (!tile_tokens_by_flags_.count(flags)){
		return;
	}

	tile_map_.SetTile(index, tile_tokens_by_flags_[flags]);
}

void TileEditor::UpdateSurroundingWallTiles(Aegis::Vec2 index)
{
	bool is_ice = tile_map_.GetTileByIndex(index)->is_slippery_;

	if (is_ice){
		ice_flags_[index.x-1][index.y] |= IceNeighborFlags::Right;
		ice_flags_[index.x+1][index.y] |= IceNeighborFlags::Left;
		ice_flags_[index.x][index.y-1] |= IceNeighborFlags::Bottom;
		ice_flags_[index.x][index.y+1] |= IceNeighborFlags::Top;
		ice_flags_[index.x-1][index.y-1] |= IceNeighborFlags::BottomRight;
		ice_flags_[index.x-1][index.y+1] |= IceNeighborFlags::TopRight;
		ice_flags_[index.x+1][index.y-1] |= IceNeighborFlags::BottomLeft;
		ice_flags_[index.x+1][index.y+1] |= IceNeighborFlags::TopLeft;
	} else {
		ice_flags_[index.x-1][index.y] &= ~IceNeighborFlags::Right;
		ice_flags_[index.x+1][index.y] &= ~IceNeighborFlags::Left;
		ice_flags_[index.x][index.y-1] &= ~IceNeighborFlags::Bottom;
		ice_flags_[index.x][index.y+1] &= ~IceNeighborFlags::Top;
		ice_flags_[index.x-1][index.y-1] &= ~IceNeighborFlags::BottomRight;
		ice_flags_[index.x-1][index.y+1] &= ~IceNeighborFlags::TopRight;
		ice_flags_[index.x+1][index.y-1] &= ~IceNeighborFlags::BottomLeft;
		ice_flags_[index.x+1][index.y+1] &= ~IceNeighborFlags::TopLeft;
	}


	std::array<int, 3> dir{-1, 0, 1};
	
	for (auto x = 0; x < dir.size(); ++x){
		for (auto y = 0; y < dir.size(); ++y){
			UpdateTile(index + Aegis::Vec2(dir[x], dir[y]));
		}
	}
}

void TileEditor::Save(int level_num)
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

	for (int row = 0; row < ice_flags_[0].size(); ++row) {
		for (int col = 0; col < ice_flags_.size(); ++col) {
			auto coord = Aegis::Vec2(col, row);

			bool is_spawn = false;
			for (const auto& [spawn, index] : tile_map_.spawn_indices_){
				if (coord == index){
					switch (spawn){
						case SpawnPoint::Brutus: file << 'p'; break;
						case SpawnPoint::Bruce: file << 's'; break;
						case SpawnPoint::Bjorn: file << 'j'; break;
					}
					is_spawn = true;
					continue;
				}
			}
			if (is_spawn) continue;
			if (tile_map_.pellet_spawn_indices_.count(coord)){
				file << 'f';
			} else {
				auto tile = tile_map_.tiles_[col][row];
				if (tile->is_solid_){
					int flags = GetSimplifiedFlags(ice_flags_[col][row]);
					file << tile_tokens_by_flags_[flags];
				} else if (tile->is_slippery_) {
					file << 'i';
				}
				else {
					file << 'g';
				}
			}
		}

		//don't print newline on last row
		if (row != tile_map_.grid_size_.y - 1) {
			file << '\n';
		}
	}
	file.close();
}
