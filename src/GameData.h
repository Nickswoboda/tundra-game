#pragma once

#include <vector>
#include <array>

class GameData
{
public:
	GameData();

	int num_levels_ = 0;
	std::vector<std::array<double, 2>> star_thresholds_;
};
