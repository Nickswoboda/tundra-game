#pragma once

#include "../../GameData.h"

#include "src/Aegis.h"

class OptionsScene : public Aegis::Scene
{
public:
	OptionsScene(GameData& game_data);
	~OptionsScene();

	void Update() override;
	void Render(float delta_time) override;
	void OnEvent(Aegis::Event& event) override;

private:
	std::shared_ptr<Aegis::Container> v_box_;
	std::shared_ptr<Aegis::Texture> bg_texture_;
};
