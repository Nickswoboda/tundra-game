#pragma once
#include "src/Aegis.h"

#include "../Tile.h"
#include "../TileMap.h"

#include <stack>

class EditCommand;
enum class SpawnPoint
{
	None,
	Bjorne,
	Brutus,
	Bruce
};

class LevelEditorScene : public Aegis::Scene
{
public:
	LevelEditorScene();
	~LevelEditorScene();

	void Update() override;
	void Render(float delta_time) override;
	void OnEvent(Aegis::Event& event) override;

	bool IsLevelValid();
	//TODO: possibly implement Vec2 hashing f(x)
	std::vector<std::vector<int>> GetReachableTileIndices(Aegis::Vec2 start_pos);
	void PreviewLevel();
	void SaveLevel();
	void Undo();

	//tiles can not be placed while a spawn is selected
	void ChangeSelectedTile(Tile::Type tile) { selected_tile_ = tile; selected_spawn_ = SpawnPoint::None;}
	void ChangeSelectedSpawn(SpawnPoint spawn) { selected_spawn_ = spawn; selected_tile_ = Tile::NumTypes;}

	std::shared_ptr<Aegis::Font> font_;
	std::shared_ptr<TileMap> tile_map_;

	Tile::Type selected_tile_ = Tile::Type::Ground;
	SpawnPoint selected_spawn_ = SpawnPoint::None;

	std::shared_ptr<Aegis::SubTexture> bjorne_tex_;
	std::shared_ptr<Aegis::SubTexture> brutus_tex_;
	std::shared_ptr<Aegis::SubTexture> bruce_tex_;

	bool show_error_msg_ = false;
	
	bool recording_ = false;
	std::stack<std::shared_ptr<EditCommand>> recorded_edits_;
	int max_undos = 100;
	std::stack<std::stack<std::shared_ptr<EditCommand>>> command_stack_;

	int level_ = 0;
	Aegis::Vec2 button_highligh_pos_;
};

class EditCommand
{
public:
	virtual ~EditCommand() {}
	virtual void Execute() = 0; 
	virtual void Undo() = 0; 
};

class TileEditCommand : public EditCommand
{
public:
	TileEditCommand(Tile& tile, Tile::Type new_type);

	void Execute() override;
	void Undo() override;
	
	Tile& tile_;
	Tile::Type prev_type_;
	Tile::Type new_type_;
};

class SpawnEditCommand : public EditCommand
{
public:
	SpawnEditCommand(TileMap& tile_map, SpawnPoint spawn_point, Aegis::Vec2 new_index);

	void Execute() override;
	void Undo() override;

	TileMap& tile_map_;
	SpawnPoint spawn_point_;
	Aegis::Vec2 prev_index_;
	Aegis::Vec2 new_index_;
};
