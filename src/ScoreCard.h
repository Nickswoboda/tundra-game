#pragma once

#include "src/Aegis.h"

class ScoreCard : public Aegis::Dialog
{
public:
	ScoreCard(const std::string& label, const Aegis::AABB& rect);

	void Render() const override;
	void OnEvent(Aegis::Event& event) override;

	void Show(double time, int stars);

	std::unique_ptr<Aegis::Container> score_container_;
	std::shared_ptr<Aegis::Label> time_label_;
	std::array<std::shared_ptr<Aegis::SpriteWidget>, 3> star_sprites_;
};
