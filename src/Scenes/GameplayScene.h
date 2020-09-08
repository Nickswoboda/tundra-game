#include "../GameObject.h"
#include "../TileMap.h"
#include <stack>
#include <memory>
enum class Direction {
	None,
	Up,
	Down,
	Left,
	Right
};

class GameplayScene : public Aegis::Scene
{
public:

	GameplayScene(int level);
	GameplayScene(std::shared_ptr<TileMap> tile_map);

	void Update() override;
	void Render(float delta_time) override;
	void OnEvent(Aegis::Event& event) override;

	void HandlePlayerMovement(int key_code);
	Aegis::Vec2 GetEnemyTargetPos(GameObject& obj);
	Aegis::Vec2 GetSlidingTargetTile(const Aegis::Vec2& start, Direction dir) const;
	Aegis::Vec2 GetTargetTileCoordBFS(const Aegis::Vec2& start, const Aegis::Vec2& end, bool sliding) const;
	void SetObjectOnGrid(GameObject& obj, const Aegis::Vec2& pos);

	std::vector<Aegis::Vec2> GetNeighborTilesMoving(const Aegis::Vec2& tile) const;
	std::vector<Aegis::Vec2> GetNeighborTilesSliding(const Aegis::Vec2& tile) const;

	void SpawnPellets();
	void SetUpLevel();
	void ResetLevel();

	Aegis::Camera ui_camera_;
	Player player_;
	Brutus brutus_;
	Bjorn bjorn_;
	std::shared_ptr<TileMap> tile_map_;
	std::vector<Pellet> pellets_;
	int num_lives_ = 3;
};
