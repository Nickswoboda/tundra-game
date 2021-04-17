 #pragma once

#include "LevelEditor.h"
#include "EditorErrorDialog.h"
#include "EditorControlsDialog.h"

class GameData;


class LevelEditorScene : public Aegis::Scene
{
public:
	//-1 indicates that level will be empty
	LevelEditorScene(GameData& game_data, int level = -1, bool is_custom = false);
	~LevelEditorScene() = default;

	void Update() override;
	void Render(float delta_time) override;
	void OnEvent(Aegis::Event& event) override;

	Error IsLevelValid();
	void PreviewLevel();
	void SaveLevel();
	
	std::shared_ptr<TileMap> tile_map_;
	std::unique_ptr<LevelEditor> level_editor_;
	std::shared_ptr<EditorErrorDialog> error_dialog_;
	std::shared_ptr<EditorControlsDialog> controls_dialog_;

	std::shared_ptr<Aegis::SpinBox> first_star_time_;
	std::shared_ptr<Aegis::SpinBox> second_star_time_;
	std::shared_ptr<Aegis::Texture> bg_texture_;

	int level_num_;

	GameData& game_data_;
};
