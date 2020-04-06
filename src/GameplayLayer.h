#pragma once

#include "src/Aegis.h"
#include "GameObject.h"
#include "Level.h"

#include <iostream>
#include <stack>

class GameplayLayer : public Aegis::Layer
{
public:
	GameplayLayer();
	
	bool HasCollided(const GameObject& obj_1, const glm::vec2& tile_index);
	void ResolveCollision(GameObject& obj_1, const glm::vec2& tile_index);
	
	void OnUpdate() override;
	void OnEvent(Aegis::Event& event);
	void OnRender(float delta_time) override;

	Player player_;
	std::stack<Level> levels_;
};