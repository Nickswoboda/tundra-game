﻿#pragma once

#include "src/Aegis.h"
#include "GameObject.h"
#include "TileMap.h"

#include <iostream>
#include <stack>

class GameplayLayer : public Aegis::Layer
{
public:
	GameplayLayer();
	
	void OnUpdate() override;
	void OnEvent(Aegis::Event& event);
	void HandlePlayerMovement(int key_code);
	void UpdatePlayerGridPosition();
	void OnRender(float delta_time) override;
	void SaveLevel();

	Player player_;
	TileMap tile_map_;
	Aegis::Texture tile_atlas_;
	int queued_movement_ = -1;
	
	
};