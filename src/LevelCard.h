#pragma once

#include "src/Aegis.h"

class LevelCard : public Aegis::Widget
{
public:
	LevelCard(int level, std::array<double, 2> star_thresholds, double fastest_time_);

	void Render() const override;
	void OnEvent(Aegis::Event& event) override;

	void SetPos(Aegis::Vec2 pos) override;
 
	std::shared_ptr<Aegis::Container> v_box_;
	int level_ = 0;
	int num_stars_earned = 0;
	double fastest_time_ = 0;
	double time_needed_for_next_star_ = 0;
	bool selected_ = false;
};

