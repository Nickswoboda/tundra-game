#pragma once 


#include "src/Aegis.h"

class GameplayScene;

class PauseMenu : public Aegis::Widget
{
public:
	PauseMenu(Aegis::AABB rect, GameplayScene& scene);
	void OnEvent(Aegis::Event& event) override;
	void Render() const override;

	std::shared_ptr<Aegis::Texture> bg_texture_;
	std::shared_ptr<Aegis::Container> v_box_;

	std::shared_ptr<Aegis::Button> resume_button_;
	std::shared_ptr<Aegis::Button> retry_button_;
	std::shared_ptr<Aegis::Button> options_button_;
	std::shared_ptr<Aegis::Button> quit_button_;
};
