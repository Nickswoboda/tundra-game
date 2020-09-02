#pragma once

#include "src/Aegis.h"

class MenuScene : public Aegis::Scene
{
public:
	MenuScene();
	~MenuScene();

	void Update() override;
	void Render(float delta_time) override;
	void OnEvent(Aegis::Event& event) override;

	std::shared_ptr<Aegis::Texture> title_background_;

	std::shared_ptr<Aegis::Sprite> title_sprite_;

	std::shared_ptr<Aegis::Container> v_box_;
};
