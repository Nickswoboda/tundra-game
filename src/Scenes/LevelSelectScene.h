#pragma once

#include "src/Aegis.h"

#include "../Button.h"
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

	std::vector<Button*> level_buttons_;
	Button* back_button_;
	Button* select_button_;



	int selected_level_ = -1;
	Aegis::Camera camera_;
};