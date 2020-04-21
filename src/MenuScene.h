#pragma once

#include "src/Aegis.h"

class MenuScene : public Aegis::Scene
{
public:
	MenuScene();

	void Update() override;
	void Render(float delta_time) override;
	void OnEvent(Aegis::Event& event) override;

};