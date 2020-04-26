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

	Aegis::Button* windowed_button_;
	Aegis::Button* fullscreen_button_;
	Aegis::Button* fullscreen_windowed_button_;
	Aegis::Button* toggle_vsync_button_;
	Aegis::Button* back_button_;

	Aegis::Button* res1280_720;
	Aegis::Button* res1600_900;
	Aegis::Button* res1920_1080;


	bool vsync_ = false;
};