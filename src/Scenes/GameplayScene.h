#include "../GameObject.h"
#include "../TileMap.h" 
#include <stack>
#include <memory>

class GameplayScene : public Aegis::Scene
{
public:

	GameplayScene(int level);
	GameplayScene(std::shared_ptr<TileMap> tile_map);

	void Init();

	void Update() override;
	void Render(float delta_time) override;
	void OnEvent(Aegis::Event& event) override;

	void HandlePlayerMovement(int key_code);
	Aegis::Vec2 GetEnemyTargetPos(GameObject& obj);
	Aegis::Vec2 GetSlidingTargetTile(const Aegis::Vec2& start, const Aegis::Vec2& dir) const;
	Aegis::Vec2 GetTargetTileCoordBFS(const Aegis::Vec2& start, const Aegis::Vec2& end, bool sliding) const;
	void SetObjectOnGrid(GameObject& obj, const Aegis::Vec2& pos);
	void ResetObjectPositions();

	std::vector<Aegis::Vec2> GetNeighborTilesMoving(const Aegis::Vec2& tile) const;
	std::vector<Aegis::Vec2> GetNeighborTilesSliding(const Aegis::Vec2& tile) const;

	void RemoveLife();
	void UpdatePelletCount();

	void SpawnPellets();
	void SetUpLevel();

	Player player_;
	Brutus brutus_;
	Bjorn bjorn_;
	std::shared_ptr<TileMap> tile_map_;
	std::vector<Pellet> pellets_;
	const int max_lives_ = 3;
	int num_lives_ = 3;
	std::shared_ptr<Aegis::SubTexture> heart_texture_;

	int pellets_collected_ = 0;
	int total_pellets_ = 0;
	std::shared_ptr<Aegis::Label> pellet_count_label_;
	Aegis::Timer countdown_;
	std::shared_ptr<Aegis::Label> countdown_label_;
	std::shared_ptr<Aegis::Dialog> dialog_;
};
