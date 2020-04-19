#pragma once

#include "src/Aegis.h"
#include "GameObject.h"
#include "TileMap.h"

#include <iostream>
#include <stack>

enum class Direction {
	None,
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
	void OnRender(float delta_time) override;

	void HandlePlayerMovement(int key_code);
	void GetEnemyTargetPos();
	Aegis::Vec2 GetTargetTileCoord(const GameObject& obj, Direction dir);
	void SetObjectOnGrid(GameObject& obj, const Aegis::Vec2& pos);

	void SpawnPellets();
	void LoadLevel(const std::string& file_path);
	void ResetLevel();
	void SaveLevel();

	Aegis::Camera camera_;
	Player player_;
	Enemy enemy_;
	std::unique_ptr<TileMap> tile_map_;
	int queued_movement_ = -1;
	std::vector<Pellet> pellets_;
	float zoom_ = 1.0f;

	
};