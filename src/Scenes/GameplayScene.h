#pragma once

#include "../GameObject.h"
#include "../TileMap.h" 
#include "../PauseMenu.h"
#include "../ScoreDialog.h"
#include "../GameData.h"
#include "../InfoDialog.h"
#include "../GameOverDialog.h"
#include "../ScoreBoard.h"

#include <stack>
#include <memory>

class GameplayScene : public Aegis::Scene
{
public:

	GameplayScene(int level, GameData& game_data);
	GameplayScene(std::shared_ptr<TileMap> tile_map, GameData& game_data);
	~GameplayScene();

	void Init();

	void Update() override;
	void Render(float delta_time) override;
	void OnEvent(Aegis::Event& event) override;
	
	void Pause();
	void Resume();

	void MovePlayer(Aegis::Vec2 dir);
	void ResetObjectPositions();

	void RemoveLife();
	void IncrementPelletCount();

	void SetUpLevel();

	Player player_;
	Brutus brutus_;
	Bjorn bjorn_;

	//-1 indicates level editor preview
	int level_ = -1;
	std::shared_ptr<TileMap> tile_map_;
	std::shared_ptr<Aegis::Texture> bg_texture_;
	std::vector<Pellet> pellets_;
	const int max_lives_ = 3;
	int num_lives_ = 3;

	std::shared_ptr<ScoreBoard> score_board_;
	int pellets_collected_ = 0;
	int total_pellets_ = 0;

	Aegis::StopWatch stopwatch_;
	Aegis::Timer countdown_;
	std::shared_ptr<Aegis::Label> countdown_label_;

	std::shared_ptr<GameOverDialog> game_over_dialog_;
	std::shared_ptr<ScoreDialog> score_dialog_;
	std::shared_ptr<InfoDialog> info_dialog_;

	std::shared_ptr<PauseMenu> pause_menu_;
	bool paused_ = false;

	Aegis::SoundID bg_music_;
	Aegis::SoundID fish_sfx_;
	Aegis::SoundID death_sfx_;
	Aegis::SoundID game_over_sfx_;
	Aegis::SoundID level_complete_sfx_;

	GameData& game_data_;

};
