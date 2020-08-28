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
		Bruce
	};

	void ChangeSelectedTile(Tile tile) { selected_tile_ = tile; selected_spawn_ = SpawnPoint::None;}
	void ChangeSelectedSpawn(SpawnPoint spawn) { selected_spawn_ = spawn; selected_tile_ = Tile::None;}

	std::unique_ptr<TileMap> tile_map_;

	Tile selected_tile_ = Tile::None;
	SpawnPoint selected_spawn_ = SpawnPoint::None;

	std::shared_ptr<Aegis::SubTexture> bjorne_tex_;
	std::shared_ptr<Aegis::SubTexture> brutus_tex_;
	std::shared_ptr<Aegis::SubTexture> bruce_tex_;


};

