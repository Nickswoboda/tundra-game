#pragma once

#include "src/Aegis.h"

class ScoreCard : public Aegis::Dialog
{
public:
	ScoreCard(const std::string& label, const Aegis::AABB& rect, const std::array<double, 2>& star_thresholds);

	void Render() const override;
	void OnEvent(Aegis::Event& event) override;

	void Show(double time);

private:
	int GetNumStarsEarned(double time_in_seconds) const;

	std::unique_ptr<Aegis::Container> score_container_;
	std::shared_ptr<Aegis::Label> time_label_;
	std::array<std::shared_ptr<Aegis::SpriteWidget>, 3> star_sprites_;
	const std::array<double, 2>& star_thresholds_;
};

//also used by GameplayScene
std::string FormatTime(double time_in_seconds);
