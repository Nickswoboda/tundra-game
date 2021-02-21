#pragma once 

#include "src/Aegis.h"

class PauseMenu : public Aegis::Widget
{
public:
	PauseMenu(Aegis::AABB rect);
	void OnEvent(Aegis::Event& event) override;
	void Render() const override;

	std::shared_ptr<Aegis::Container> v_box_;

	std::shared_ptr<Aegis::Button> continue_button_;
	std::shared_ptr<Aegis::Button> retry_button_;
	std::shared_ptr<Aegis::Button> options_button_;
	std::shared_ptr<Aegis::Button> quit_button_;
};
