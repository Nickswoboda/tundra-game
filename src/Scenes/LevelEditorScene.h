#pragma once
#include "src/Aegis.h"

#include "../TileMap.h"

class LevelEditorScene : public Aegis::Scene
{
public:
	LevelEditorScene();
	~LevelEditorScene();

	void Update() override;
	void Render(float delta_time) override;
	void OnEvent(Aegis::Event& event) override;

	std::unique_ptr<TileMap> tile_map_;
	std::shared_ptr<Aegis::Button> back_button_;

};

