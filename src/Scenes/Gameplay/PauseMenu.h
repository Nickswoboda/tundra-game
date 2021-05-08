#pragma once 

#include "src/Aegis.h"

class GameplayScene;

class PauseMenu : public Aegis::Widget
{
public:
	PauseMenu(GameplayScene& scene);
	void OnEvent(Aegis::Event& event) override;
	void Render() const override;

	Aegis::VContainer v_box_;
};
