#pragma once
#include "src/Aegis.h"

#include "../TileMap.h"

class LevelEditorScene : public Aegis::Scene
{
public:
	LevelEditorScene();
	~LevelEditorScene();

	void Update() override;
	void Render(float delta_time) override;
	void OnEvent(Aegis::Event& event) override;

	enum class Tile
	{
		None,
		Ground,
		Ice,
		Wall
	};
	enum class SpawnPoint
	{
		None,
		Bjorne,
		Brutus,
		Player
	};

	void ChangeSelectedTile(Tile tile) { selected_tile_ = tile; selected_spawn_ = SpawnPoint::None;}
	void ChangeSelectedSpawn(SpawnPoint spawn) { selected_spawn_ = spawn; selected_tile_ = Tile::None;}

	std::unique_ptr<TileMap> tile_map_;
	std::shared_ptr<Aegis::Button> back_button_;

	std::shared_ptr<Aegis::Button> ground_tile_button_;
	std::shared_ptr<Aegis::Button> ice_tile_button_;
	std::shared_ptr<Aegis::Button> wall_tile_button_;
	std::shared_ptr<Aegis::Button> brutus_button_;
	std::shared_ptr<Aegis::Button> bjorne_button_;
	std::shared_ptr<Aegis::Button> player_button_;

	Tile selected_tile_ = Tile::None;
	SpawnPoint selected_spawn_ = SpawnPoint::None;

};

