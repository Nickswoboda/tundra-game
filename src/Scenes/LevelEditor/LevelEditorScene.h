 #pragma once

#include "LevelEditor.h"
#include "../../ErrorDialog.h"
#include "EditorControlsDialog.h"

class GameData;


class LevelEditorScene : public Aegis::Scene
{
public:
	//-1 indicates that level will be empty
	LevelEditorScene(GameData& game_data, int level = -1);
	~LevelEditorScene() = default;

	void Update() override;
	void Render(float delta_time) override;
	void OnEvent(Aegis::Event& event) override;

	Error IsLevelValid();
	void PreviewLevel();
	void SaveLevel();
	
	std::shared_ptr<TileMap> tile_map_;
	std::unique_ptr<LevelEditor> level_editor_;
	std::shared_ptr<ErrorDialog> error_dialog_;
	std::shared_ptr<EditorControlsDialog> controls_dialog_;

	std::shared_ptr<Aegis::SpinBox> two_star_spinbox_;
	std::shared_ptr<Aegis::SpinBox> three_star_spinbox_;
	std::shared_ptr<Aegis::Texture> bg_texture_;

	int level_num_;

	GameData& game_data_;
};

