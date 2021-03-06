 #pragma once

#include "../EditCommand.h"

#include <stack>

class GameData;

class LevelEditorScene : public Aegis::Scene
{
public:
	//-1 indicates that level will be empty
	LevelEditorScene(GameData& game_data, int level = -1);
	~LevelEditorScene();

	void Update() override;
	void Render(float delta_time) override;
	void OnEvent(Aegis::Event& event) override;

	bool IsLevelValid();
	void PreviewLevel();
	void SaveLevel();
	
	void UpdateObjectPositions();
	void Undo();

	//tiles can not be placed while a spawn is selected
	void ChangeSelectedTile(const Tile& tile); 
	void ChangeSelectedSpawn(SpawnPoint spawn);

	std::shared_ptr<TileMap> tile_map_;

	const Tile* selected_tile_ = nullptr;
	SpawnPoint selected_spawn_ = SpawnPoint::None;

	std::shared_ptr<Aegis::Font> font_;
	Aegis::Sprite bjorn_sprite_;
	Aegis::Sprite brutus_sprite_;
	Aegis::Sprite bruce_sprite_;

	std::shared_ptr<Aegis::Label> tile_text_;
	std::shared_ptr<Aegis::Label> spawn_text_;

	bool show_error_msg_ = false;
	
	bool recording_edits_ = false;
	std::stack<std::shared_ptr<EditCommand>> recorded_edits_;
	std::stack<std::stack<std::shared_ptr<EditCommand>>> edit_stack_;

	int level_num_;

	GameData& game_data_;
};

