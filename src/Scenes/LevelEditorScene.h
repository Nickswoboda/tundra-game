#pragma once

#include "../EditCommand.h"

#include <stack>

class LevelEditorScene : public Aegis::Scene
{
public:
	LevelEditorScene();
	~LevelEditorScene();

	void Update() override;
	void Render(float delta_time) override;
	void OnEvent(Aegis::Event& event) override;

	bool IsLevelValid();
	void PreviewLevel();
	void SaveLevel();
	
	void Undo();

	//tiles can not be placed while a spawn is selected
	void ChangeSelectedTile(const Tile& tile) { selected_tile_ = &tile; selected_spawn_ = SpawnPoint::None;}
	void ChangeSelectedSpawn(SpawnPoint spawn) { selected_spawn_ = spawn; selected_tile_ = nullptr;}

	std::shared_ptr<TileMap> tile_map_;

	const Tile* selected_tile_ = nullptr;
	SpawnPoint selected_spawn_ = SpawnPoint::None;

	std::shared_ptr<Aegis::Font> font_;
	std::shared_ptr<Aegis::SubTexture> bjorn_tex_;
	std::shared_ptr<Aegis::SubTexture> brutus_tex_;
	std::shared_ptr<Aegis::SubTexture> bruce_tex_;

	bool show_error_msg_ = false;
	
	bool recording_edits_ = false;
	std::stack<std::shared_ptr<EditCommand>> recorded_edits_;
	std::stack<std::stack<std::shared_ptr<EditCommand>>> edit_stack_;

	int level_ = 0;
};

