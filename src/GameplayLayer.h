#pragma once

#include "src/Aegis.h"
#include "GameObject.h"
#include "TileMap.h"

#include <iostream>
#include <stack>

class GameplayLayer : public Aegis::Layer
{
public:
	GameplayLayer();
	
	bool HasCollided(const GameObject& obj_1, int tile_x, int tile_y);
	void ResolveCollision(GameObject& obj_1, int tile_x, int tile_y);
	
	void OnUpdate() override;
	void OnEvent(Aegis::Event& event);
	void OnRender(float delta_time) override;

	Player player_;
	TileMap tile_map_;
};