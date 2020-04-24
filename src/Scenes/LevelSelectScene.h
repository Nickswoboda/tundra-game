#pragma once

#include "src/Aegis.h"

#include <memory>
class LevelSelectScene : public Aegis::Scene
{
public:
	LevelSelectScene();
	~LevelSelectScene();

	void Update() override;
	void Render(float delta_time) override;
	void OnEvent(Aegis::Event& event) override;

	std::shared_ptr<Aegis::Font> title_font_;
	std::shared_ptr<Aegis::Font> button_font_;

	std::vector<Aegis::Button*> level_buttons_;
	Aegis::Button* back_button_;
	Aegis::Button* select_button_;

	int selected_level_ = -1;
};