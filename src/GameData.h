#pragma once

#include <vector>
#include <array>

class GameData
{
public:
	GameData();

	void Load();
	void Save() const;

	int num_levels_ = 0;
	int num_custom_levels_ = 0;
	int levels_completed_ = 0;
	std::vector<std::array<double, 2>> star_thresholds_;
	std::vector<double> record_times_;
	bool first_time_playing_ = true;
};
