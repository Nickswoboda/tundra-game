#pragma once

#include "src/Aegis.h"

class LevelCard : public Aegis::Widget
{
public:
	LevelCard(int level, const std::array<double, 2>& star_thresholds, const double fastest_time_);

	void Render() const override;
	void OnEvent(Aegis::Event& event) override;

	void SetPos(Aegis::Vec2 pos) override;
 
	std::shared_ptr<Aegis::Container> v_box_;
	int level_ = 0;
	bool selected_ = false;
};

