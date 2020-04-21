#pragma once

#include "src/Aegis.h"
#include "GameplayState.h"

#include <iostream>
#include <stack>

class GameplayLayer : public Aegis::Layer
{
public:
	GameplayLayer();
	
	void OnUpdate() override;
	void OnEvent(Aegis::Event& event);
	void OnRender(float delta_time) override;

	std::stack<std::unique_ptr<GameState>> states_;
};