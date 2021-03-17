#pragma once

#include "src/Aegis.h"

#include <memory>

class GameData;
class LevelSelectScene : public Aegis::Scene
{
public:
	LevelSelectScene(GameData& game_data);
	~LevelSelectScene();

	void Update() override;
	void Render(float delta_time) override;
	void OnEvent(Aegis::Event& event) override;

	std::shared_ptr<Aegis::Font> title_font_;
	std::shared_ptr<Aegis::Font> button_font_;

	int selected_level_ = -1;
};
