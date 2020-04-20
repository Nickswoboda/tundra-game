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
	Aegis::Vec2 GetTargetTileCoord(const Aegis::Vec2& start, Direction dir) const;
	Aegis::Vec2 GetTargetTileCoordBFS(const Aegis::Vec2& start, const Aegis::Vec2& end, bool sliding) const;
	void SetObjectOnGrid(GameObject& obj, const Aegis::Vec2& pos);

	std::vector<Aegis::Vec2> GetNeighborTilesMoving(const Aegis::Vec2& tile) const;
	std::vector<Aegis::Vec2> GetNeighborTilesSliding(const Aegis::Vec2& tile) const;

	void SpawnPellets();
	void LoadLevel(const std::string& file_path);
	void ResetLevel();
	void SaveLevel() const;

	Aegis::Camera world_camera_;
	Aegis::Camera ui_camera_;
	Player player_;
	Brutus brutus_;
	Bjorne bjorne_;
	std::unique_ptr<TileMap> tile_map_;
	int queued_movement_ = -1;
	std::vector<Pellet> pellets_;
	float zoom_ = 1.0f;

	
};