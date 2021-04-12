#pragma once

#include "TileMap.h"
#include "EditCommand.h"
#include "TileEditor.h"

#include <stack>

class LevelEditor
{
public:
	LevelEditor(TileMap& tile_map);

	void OnMouseClick(int action, int button);
	void OnKeyRelease(int key);
	void OnMouseMove();

	void Render(float dt);

	void Undo();
	void ResetTileMap();

private:
	bool RequestEdit(EditType type);
	void PushEditsOntoStack();

	bool GrabEntityAtIndex(Aegis::Vec2 index);
	void MoveGrabbedSprite();
	
	void UpdateSpritePositions();

	TileMap& tile_map_;
	TileEditor tile_editor_;
	char current_tile_token_;

	std::shared_ptr<Aegis::Texture> sprite_sheet_;
	Aegis::Vec4 fish_texture_coords_;

	std::unordered_map<SpawnPoint, Aegis::Sprite> sprites_;
	SpawnPoint current_spawn_ = SpawnPoint::None;

	bool recording_ = false;
	std::stack<std::shared_ptr<EditCommand>> recorded_edits_;
	std::stack<std::stack<std::shared_ptr<EditCommand>>> edit_stack_;
};
