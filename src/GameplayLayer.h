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
	void GetBrutusTargetPos();
	void GetBjorneTargetPos();
	Aegis::Vec2 GetTargetTileCoord(const Aegis::Vec2& start, Direction dir);
	Aegis::Vec2 GetTargetTileCoordBFS(const Aegis::Vec2& start, const Aegis::Vec2& end, bool sliding);
	void SetObjectOnGrid(GameObject& obj, const Aegis::Vec2& pos);

	std::vector<Aegis::Vec2> GetNeighborTilesMoving(const Aegis::Vec2& tile);
	std::vector<Aegis::Vec2> GetNeighborTilesSliding(const Aegis::Vec2& tile);

	void SpawnPellets();
	void LoadLevel(const std::string& file_path);
	void ResetLevel();
	void SaveLevel();

	Aegis::Camera camera_;
	Player player_;
	Brutus brutus_;
	Bjorne bjorne_;
	std::unique_ptr<TileMap> tile_map_;
	int queued_movement_ = -1;
	std::vector<Pellet> pellets_;
	float zoom_ = 1.0f;

	
};