#pragma once

#include "src/Aegis.h"

#include "Button.h"

class MenuScene : public Aegis::Scene
{
public:
	MenuScene();

	void Update() override;
	void Render(float delta_time) override;
	void OnEvent(Aegis::Event& event) override;

	std::shared_ptr<Aegis::Font> title_font_;
	std::shared_ptr<Aegis::Font> button_font_;

	Button* new_game_button_;
	Button* level_select_button_;
	Button* options_button_;
	Button* exit_button_;

	Aegis::Camera camera_;

};