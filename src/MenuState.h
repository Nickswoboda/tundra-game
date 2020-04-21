#pragma once

#include "GameState.h"

#include <stack>
class MenuState : public GameState
{
public:
	MenuState(std::stack<std::unique_ptr<GameState>>& states);

	void Update() override;
	void Render(float delta_time) override;
	void OnEvent(Aegis::Event& event) override;

	std::stack<std::unique_ptr<GameState>>& states_;
};