#pragma once

#include "src/Aegis.h"

#include "../Particle.h"

class GameData;

class MenuScene : public Aegis::Scene
{
public:
	MenuScene(GameData& game_data);

	void Update() override;
	void Render(float delta_time) override;
	void OnEvent(Aegis::Event& event) override;

	std::shared_ptr<Aegis::Texture> title_background_;
	std::shared_ptr<Aegis::Container> v_box_;

	std::shared_ptr<ParticleEngine> snow_engine_;

	Aegis::SoundID bg_music_;
	GameData& game_data_;
};
