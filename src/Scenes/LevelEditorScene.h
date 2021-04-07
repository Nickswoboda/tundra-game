 #pragma once

#include "../EditCommand.h"

#include <stack>

class GameData;

class LevelEditorScene : public Aegis::Scene
{
public:
	//-1 indicates that level will be empty
	LevelEditorScene(GameData& game_data, int level = -1, bool is_custom = false);
	~LevelEditorScene();

	void Update() override;
	void Render(float delta_time) override;
	void OnEvent(Aegis::Event& event) override;

	bool IsLevelValid();
	void PreviewLevel();
	void SaveLevel();
	
	void UpdateObjectPositions();
	void Undo();

	bool GrabEntityAtIndex(Aegis::Vec2 index);

	std::shared_ptr<TileMap> tile_map_;

	const Tile* selected_tile_ = nullptr;

	std::unordered_map<SpawnPoint, Aegis::Sprite> sprites_;

	SpawnPoint spawn_being_edited_ = SpawnPoint::None;
	Aegis::Sprite* dragged_sprite_ = nullptr;

	std::shared_ptr<Aegis::Texture> tex_atlas_;
	Aegis::Vec4 fish_texture_coords_;

	bool show_error_msg_ = false;
	
	bool recording_edits_ = false;
	std::stack<std::shared_ptr<EditCommand>> recorded_edits_;
	std::stack<std::stack<std::shared_ptr<EditCommand>>> edit_stack_;

	int level_num_;

	GameData& game_data_;
};

