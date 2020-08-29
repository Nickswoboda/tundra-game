#pragma once
#include "src/Aegis.h"

#include "../Tile.h"
#include "../TileMap.h"

#include <stack>

struct EditCommand
{
	Aegis::Vec2 tile_pos_;
	Tile::Type type_;
	
};
class LevelEditorScene : public Aegis::Scene
{
public:
	LevelEditorScene();
	~LevelEditorScene();

	void Update() override;
	void Render(float delta_time) override;
	void OnEvent(Aegis::Event& event) override;

	void SaveLevel();
	void Undo();

	enum class SpawnPoint
	{
		None,
		Bjorne,
		Brutus,
		Bruce
	};

	//tiles can not be placed while a spawn is selected
	void ChangeSelectedTile(Tile::Type tile) { selected_tile_ = tile; selected_spawn_ = SpawnPoint::None;}
	void ChangeSelectedSpawn(SpawnPoint spawn) { selected_spawn_ = spawn;}

	std::unique_ptr<TileMap> tile_map_;

	Tile::Type selected_tile_ = Tile::Type::Ground;
	SpawnPoint selected_spawn_ = SpawnPoint::None;

	std::shared_ptr<Aegis::SubTexture> bjorne_tex_;
	std::shared_ptr<Aegis::SubTexture> brutus_tex_;
	std::shared_ptr<Aegis::SubTexture> bruce_tex_;
	
	bool recording_ = false;
	std::stack<EditCommand> recorded_edits_;
	int max_undos = 100;
	std::stack<std::stack<EditCommand>> undo_stack_;

};

