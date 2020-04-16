#pragma once

#include "src/Aegis.h"
#include "GameObject.h"
#include "TileMap.h"

#include <iostream>
#include <stack>

enum class Direction {
	Up,
	Down,
	Left,
	Right
};

class GameplayLayer : public Aegis::Layer
{
public:
	GameplayLayer();
	
	void OnUpdate() override;
	void OnEvent(Aegis::Event& event);
	void HandlePlayerMovement(int key_code);
	void UpdateGridPosition(GameObject& obj);
	void OnRender(float delta_time) override;
	void SaveLevel();
	void SpawnPellets();
	void LoadLevel(const std::string& file_path);
	void SetPosOnGrid(GameObject& obj, const Aegis::Vec2& pos);
	Aegis::Vec2 GetTargetTile(const GameObject& obj, Direction dir);
	Aegis::Vec2 GetEnemyTargetPos();

	Player player_;
	Enemy enemy_;
	std::unique_ptr<TileMap> tile_map_;
	int queued_movement_ = -1;
	std::vector<Pellet> pellets_;
	float zoom_ = 1.0f;

	
};