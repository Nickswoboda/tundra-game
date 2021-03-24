#pragma once

#include "src/Aegis.h"

#include <memory>

class GameData;
class LevelSelectScene : public Aegis::Scene
{
public:
	LevelSelectScene(GameData& game_data, bool show_custom = false);
	~LevelSelectScene();

	void Update() override;
	void Render(float delta_time) override;
	void OnEvent(Aegis::Event& event) override;

	std::shared_ptr<Aegis::Texture> bg_;

	std::shared_ptr<Aegis::Container> v_box_;

	int selected_level_ = -1;
private:
	void CreateLevelCards(GameData& game_data, bool custom);
};
