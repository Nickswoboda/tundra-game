#pragma once

#include "src/Aegis.h"

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

	Aegis::Checkbox* vsync_checkbox_;
	Aegis::Button* back_button_;

	Aegis::Dropdown* screen_mode_dropdown_;
	Aegis::Dropdown* resolution_dropdown_;

	bool vsync_ = false;
};
