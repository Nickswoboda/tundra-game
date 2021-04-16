#pragma once

#include <vector>
#include <array>

class GameData
{
public:
	GameData();

	void Load();
	void Save() const;

	bool first_time_playing_ = true;

	int num_levels_ = 0;
	int levels_completed_ = 0;
	std::vector<std::array<int, 2>> star_thresholds_;
	std::vector<double> record_times_;

	int num_custom_levels_ = 0;
	std::vector<std::array<int, 2>> custom_star_thresholds_;
	std::vector<double> custom_record_times_;
};
