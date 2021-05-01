#include "LevelEditor.h"

Aegis::Vec4 GetFishTextureCoords(const Aegis::Texture& texture)
{
	Aegis::AABB subtex_rect = {96, 96, 32, 32};
	float x1 = subtex_rect.pos.x / texture.size_.x;
	float y1 = subtex_rect.pos.y / texture.size_.y;
	float x2 = (subtex_rect.pos.x + subtex_rect.size.x) / texture.size_.x;
	float y2 = (subtex_rect.pos.y + subtex_rect.size.y) / texture.size_.y;
	return { x1, y1, x2, y2 };
}

LevelEditor::LevelEditor(TileMap& tile_map)
	:tile_map_(tile_map), tile_editor_(tile_map)
{
	sprite_sheet_ = Aegis::TextureManager::Load("assets/textures/tile_map.png");
	fish_texture_coords_ = GetFishTextureCoords(*sprite_sheet_);

	sprites_[SpawnPoint::Bruce] = Aegis::Sprite(sprite_sheet_, { 0, 96, 32, 32 });
	sprites_[SpawnPoint::Brutus] = Aegis::Sprite(sprite_sheet_, { 32, 96, 32, 32 });
	sprites_[SpawnPoint::Bjorn] = Aegis::Sprite(sprite_sheet_, { 64, 96, 32, 32 });

	for (auto& [spawn, sprite] : sprites_){
		sprite.position_ = tile_map_.spawn_indices_[spawn] * 32;
	}
}

void LevelEditor::Save(int num_level)
{
	tile_editor_.Save(num_level);
}

void LevelEditor::Render(float dt)
{
	tile_map_.Render();
	tile_editor_.DrawGridLines();

	for (const auto& [spawn, sprite] : sprites_){
		sprite.Draw();
	}

	for (auto& index : tile_map_.pellet_spawn_indices_) {
		Aegis::DrawSubTexture((index * 32) + Aegis::Vec2(8,8), {16, 16}, *sprite_sheet_, fish_texture_coords_);
	}

	for (const auto& index : invalid_tiles_) {
		Aegis::DrawQuad(index * 32, {32, 32}, {1.0f, 0.0f, 0.0f, 0.3f});
	}
}

void LevelEditor::OnKeyRelease(int key)
{
	if (recording_) return;

	if (key == AE_KEY_F){
		RequestEdit(EditType::Fish);
		PushEditsOntoStack();
	}
	else if (key == AE_KEY_G){
		current_tile_token_ = 'g';
		RequestEdit(EditType::Tile);
		PushEditsOntoStack();
	}
}

void LevelEditor::OnMouseMove()
{
	if (current_spawn_ != SpawnPoint::None){
		MoveGrabbedSprite();
	} else if (recording_){
		RequestEdit(EditType::Tile);
	}
}

void LevelEditor::OnMouseClick(int action, int button)
{
	if (action == AE_BUTTON_RELEASE){
		if (current_spawn_ != SpawnPoint::None){
			RequestEdit(EditType::Spawn);
			UpdateSpritePositions();
		}
		PushEditsOntoStack();
	} 

	if (action == AE_BUTTON_PRESS){
		if (button == AE_MOUSE_BUTTON_RIGHT){
			current_tile_token_ = 'w';
			RequestEdit(EditType::Tile);
		} else if (button == AE_MOUSE_BUTTON_LEFT){
			if (!RequestEdit(EditType::Spawn)){
				current_tile_token_ = 'i';
				RequestEdit(EditType::Tile);
			} else {
				UpdateSpritePositions();
			}
		}
	}
	return;
}
bool LevelEditor::RequestEdit(EditType edit_type)
{
	auto mouse_pos = Aegis::Application::GetWindow().GetMousePos() - Aegis::Vec2{270, 24};
	const Tile* tile = tile_map_.GetTileByPos(mouse_pos);

	if (!tile) return false;
	Aegis::Vec2 tile_index = tile_map_.GetGridIndexByPos(mouse_pos);

	std::shared_ptr<EditCommand> command;
	std::shared_ptr<EditCommand> remove_fish;

	switch (edit_type) {
		case EditType::Tile: {
			if (current_tile_token_ != 'g'){
				recording_ = true;
			}
			auto& new_tile = tile_map_.tiles_by_token[current_tile_token_];
			if (tile != &new_tile && tile_index.x > 0 && tile_index.y > 0
				&& tile_index.x < tile_map_.grid_size_.x - 1
				&& tile_index.y < tile_map_.grid_size_.y - 1){
				command = std::make_shared<TileEditCommand>(tile_editor_, tile_index, new_tile);
				if (tile_map_.pellet_spawn_indices_.count(tile_index)){
						remove_fish = std::make_shared<FishEditCommand>(tile_map_, tile_index);
				}
			}

			break;
		}
		case EditType::Spawn: { 
			if (current_spawn_ == SpawnPoint::None){
				return (GrabEntityAtIndex(tile_index));
			}

			if (tile->is_slippery_){
				bool tile_occupied = false;
				for (const auto& [key, index] : tile_map_.spawn_indices_){
					if (index == tile_index){
						tile_occupied = true;
					}
				}
				if (!tile_occupied){
					command = std::make_shared<SpawnEditCommand>(tile_map_, current_spawn_, tile_index);
					if (tile_map_.pellet_spawn_indices_.count(tile_index)){
							remove_fish = std::make_shared<FishEditCommand>(tile_map_, tile_index);
					}
				}
			}
			current_spawn_ = SpawnPoint::None;
			break;
		}
		case EditType::Fish: { 
			if (tile->is_slippery_){
				command = std::make_shared<FishEditCommand>(tile_map_, tile_index);
			}
			break;
		}
	}

	if (command){
		command->Execute();
		recorded_edits_.push(command);
		if (remove_fish){
			remove_fish->Execute();
			recorded_edits_.push(remove_fish);
		}

		invalid_tiles_.clear();
		return true;
	} else {
		return false;
	}
}

void LevelEditor::Undo()
{
	if (edit_stack_.empty()) return;

	else{
		auto recorded_commands = edit_stack_.top();
		edit_stack_.pop();

		while(!recorded_commands.empty()){
			recorded_commands.top()->Undo();
			recorded_commands.pop();
		}

		UpdateSpritePositions();
	}
}

void LevelEditor::PushEditsOntoStack()
{
	if (!recorded_edits_.empty()){
		edit_stack_.push(recorded_edits_);
	}

	while (!recorded_edits_.empty()){
		recorded_edits_.pop();
	}
	recording_ = false;
}

bool LevelEditor::GrabEntityAtIndex(Aegis::Vec2 index)
{
	for (const auto& [spawn, idx] : tile_map_.spawn_indices_){
		if (idx == index){
			current_spawn_ = spawn;

			MoveGrabbedSprite();
			return true;
		}
	}

	return false;
}


void LevelEditor::MoveGrabbedSprite()
{
	auto mouse_pos = Aegis::Application::GetWindow().GetMousePos() - Aegis::Vec2{270, 24};
	auto& sprite = sprites_[current_spawn_];
	sprite.position_ = mouse_pos - (sprite.GetRect().size / 2);
}

void LevelEditor::ResetTileMap()
{
	tile_map_.Clear();
	UpdateSpritePositions();

	for (auto& row : tile_editor_.ice_flags_){
		for (auto& col : row){
			col = 0;
		}
	}
}

void LevelEditor::UpdateSpritePositions()
{
	for (auto& [spawn, sprite] : sprites_){
		sprite.position_ = tile_map_.spawn_indices_[spawn] * 32;
	}
}

bool LevelEditor::TilesAreValid()
{
	invalid_tiles_ = tile_editor_.GetInvalidTiles();
	return invalid_tiles_.empty();
}
