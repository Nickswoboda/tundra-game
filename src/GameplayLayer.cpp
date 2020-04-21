#include "GameplayLayer.h"

GameplayLayer::GameplayLayer()
{
	states_.emplace(std::make_unique<GameplayState>(states_));
}

void GameplayLayer::OnUpdate()
{
	states_.top()->Update();
}
void GameplayLayer::OnEvent(Aegis::Event& event)
{
	states_.top()->OnEvent(event);
}

void GameplayLayer::OnRender(float delta_time)
{
	states_.top()->Render(delta_time);
}