#include "../GameObject.h"
#include "../TileMap.h" 
#include "../PauseMenu.h"

#include <stack>
#include <memory>

class GameplayScene : public Aegis::Scene
{
public:

	GameplayScene(int level);
	GameplayScene(std::shared_ptr<TileMap> tile_map);
	~GameplayScene();

	void Init();

	void Update() override;
	void Render(float delta_time) override;
	void OnEvent(Aegis::Event& event) override;
	
	void Pause(bool pause);

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

	int level_ = 0;
	int num_levels_ = 0;
	std::shared_ptr<TileMap> tile_map_;
	std::shared_ptr<Aegis::Texture> bg_texture_;
	std::vector<Pellet> pellets_;
	const int max_lives_ = 3;
	int num_lives_ = 3;
	std::array<std::shared_ptr<Aegis::SpriteWidget>, 3> heart_widgets_;

	int pellets_collected_ = 0;
	int total_pellets_ = 0;
	std::shared_ptr<Aegis::Label> pellet_count_label_;
	Aegis::Timer countdown_;
	std::shared_ptr<Aegis::Label> countdown_label_;

	std::shared_ptr<Aegis::Dialog> game_over_dialog_;
	std::shared_ptr<Aegis::Dialog> level_complete_dialog_;
	std::shared_ptr<Aegis::Dialog> game_complete_dialog_;

	std::shared_ptr<PauseMenu> pause_menu_;
	bool paused_ = false;

	Aegis::SoundID bg_music_;
	Aegis::SoundID fish_sfx_;
	Aegis::SoundID death_sfx_;
	Aegis::SoundID game_over_sfx_;
	Aegis::SoundID level_complete_sfx_;

};
