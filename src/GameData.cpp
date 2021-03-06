#include "GameData.h"

#include "src/Aegis.h"
#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

GameData::GameData()
{
	Load();
};

std::string GetOptionValue(const std::string& option)
{
	std::ifstream file("assets/game_data.txt");
	std::string data;
	if (file) {
		file.seekg(0, std::ios::end);
		size_t size = file.tellg();
		if (size != -1) {
			data.resize(size);
			file.seekg(0, std::ios::beg);
			file.read(&data[0], size);
		}
	}

	int option_start = data.find(option); 
	if (option_start == std::string::npos){
		return {};
	}
	// + 1 for added colon at end of option in file
	int data_start = option_start + option.size() + 1;

	int end_pos = data.find('\n', option_start);

	return data.substr(data_start, end_pos - data_start);
}

int GetNumLevels()
{
	int num_levels = 0;
	std::string level_file = "assets/levels/level_" + std::to_string(num_levels+1) + ".txt";
	while (std::filesystem::exists(level_file)) {
		++num_levels;
		level_file = "assets/levels/level_" + std::to_string(num_levels+1) + ".txt";
	}

	return num_levels;
}

std::vector<std::array<double, 2>> GetStarThresholds(int num_levels)
{
	auto values = GetOptionValue("star_thresholds");

	std::vector<std::array<double, 2>> thresholds;
	thresholds.resize(num_levels);

	if (values.empty()) {
		for (int i = 0; i < num_levels; ++i) {
			thresholds[i] = { 60, 120 };
		}
	}
	else {
		std::stringstream input(values);

		int level = 0;
		while (input.good() && level < num_levels) {
			std::array<double, 2> temp;
			input >> temp[0];
			input >> temp[1];
			thresholds[level] = temp;
			++level;
		}
	}

	return thresholds; 
}

void GameData::Load()
{
	num_levels_ = GetNumLevels();
	star_thresholds_ = GetStarThresholds(num_levels_);

	auto first_time = GetOptionValue("first_time");

	first_time_playing_ = first_time.empty() ? true : (first_time != "1"); 
}

void GameData::Save() const
{
	std::ofstream file("assets/game_data.txt");

	file << "first_time:" << first_time_playing_ << "\n";
	file << "star_thresholds:";
	for (auto& level : star_thresholds_) {
		file << level[0] << " " << level[1] << " ";
	}
	file << "\n";
}

