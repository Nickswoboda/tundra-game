#pragma once

#include "src/Aegis.h"

class MenuScene : public Aegis::Scene
{
public:
	MenuScene();
	~MenuScene();

	void Update() override;
	void Render(float delta_time) override;
	void OnEvent(Aegis::Event& event) override;

	std::shared_ptr<Aegis::Font> title_font_;
	std::shared_ptr<Aegis::Font> button_font_;

	Aegis::Container* v_box_;
	Aegis::Button* new_game_button_;
	Aegis::Button* level_select_button_;
	Aegis::Button* options_button_;
	Aegis::Button* exit_button_;
};
