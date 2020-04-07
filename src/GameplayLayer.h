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
	
	void ResolveCollision(GameObject& obj_1, const Tile& tile);
	
	void OnUpdate() override;
	void OnEvent(Aegis::Event& event);
	void OnRender(float delta_time) override;

	Player player_;
	TileMap tile_map_;
};