#include "GameState.h"

#include "GameObject.h"
#include "TileMap.h"
#include <stack>
#include <memory>
enum class Direction {
	None,
	Up,
	Down,
	Left,
	Right
};

class GameplayState : public GameState
{
public:

	GameplayState(std::stack<std::unique_ptr<GameState>>& states);

	void Update() override;
	void Render(float delta_time) override;
	void OnEvent(Aegis::Event& event) override;

	void HandlePlayerMovement(int key_code);
	void GetEnemyTargetPos(GameObject& obj);
	Aegis::Vec2 GetSlidingTargetTile(const Aegis::Vec2& start, Direction dir) const;
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
	int num_lives_ = 3;
	std::stack<std::unique_ptr<GameState>>& states_;
};