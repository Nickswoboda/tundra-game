#pragma once

#include "src/Aegis.h"

#include "../Dropdown.h"

class OptionsScene : public Aegis::Scene
{
public:
	OptionsScene();
	~OptionsScene();

	void Update() override;
	void Render(float delta_time) override;
	void OnEvent(Aegis::Event& event) override;

	void SetResolution(int x, int y);

	std::shared_ptr<Aegis::Font> title_font_;
	std::shared_ptr<Aegis::Font> button_font_;

	Aegis::Button* toggle_vsync_button_;
	Aegis::Button* back_button_;

	Dropdown* screen_mode_dropdown_;
	Dropdown* resolution_dropdown_;

	bool vsync_ = false;
};