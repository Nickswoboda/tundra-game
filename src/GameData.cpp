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
	// + 2 for added ": " at end of option in file
	int data_start = option_start + option.size() + 2;

	int end_pos = data.find('\n', option_start);

	return data.substr(data_start, end_pos - data_start);
}

int GetNumLevels(bool custom)
{
	int num_levels = 0;

	std::string prefix = custom ? "assets/levels/custom_level_" : "assets/levels/level_";
	std::string level_file = prefix + std::to_string(num_levels+1) + ".txt";
	while (std::filesystem::exists(level_file)) {
		++num_levels;
		level_file = prefix + std::to_string(num_levels+1) + ".txt";
	}

	return num_levels;
}

std::vector<std::array<int, 2>> GetStarThresholds(int num_levels, bool custom)
{
	auto values = GetOptionValue(custom ? "custom_star_thresholds" : "star_thresholds");

	std::vector<std::array<int, 2>> thresholds;
	thresholds.resize(num_levels);

	if (values.size() != num_levels) {
		for (int i = 0; i < num_levels; ++i) {
			thresholds[i] = { 60, 120 };
		}
	}
	else {
		std::stringstream input(values);

		int level = 0;
		while (input.good() && level < num_levels) {
			std::array<int, 2> temp;
			input >> temp[0];
			input >> temp[1];
			thresholds[level] = temp;
			++level;
		}
	}

	return thresholds; 
}

std::vector<double> GetRecordTimes(int num_levels, bool custom)
{
	auto values = GetOptionValue(custom ? "custom_record_times" : "record_times");
	std::vector<double> records(num_levels, -1);

	int level = 0;
	std::stringstream input(values);
	while (input.good() && level < num_levels){
		input >> records[level];
		++level;
	}

	return records; 
}
void GameData::Load()
{
	num_levels_ = GetNumLevels(false);
	num_custom_levels_ = GetNumLevels(true);
	star_thresholds_ = GetStarThresholds(num_levels_, true);
	custom_star_thresholds_ = GetStarThresholds(num_custom_levels_, true);

	std::string completed = GetOptionValue("levels_completed");
	levels_completed_ = completed.empty() ? 0 : std::stoi(completed);

	record_times_ = GetRecordTimes(num_levels_, false);
	custom_record_times_ = GetRecordTimes(num_custom_levels_, true);

	std::string first_time = GetOptionValue("first_time");
	first_time_playing_ = first_time.empty() ? true : (first_time == "1"); 

	std::string volume = GetOptionValue("volume");
	volume_ = volume.empty() ? 100 : std::stoi(volume);

	std::string muted = GetOptionValue("muted");
	muted_ = muted.empty() ? false : (muted == "1"); 

	Aegis::AudioPlayer::SetMasterVolume(muted_ ? 0 : volume_);
	std::string vsync = GetOptionValue("vsync");
	vsync_ = vsync.empty() ? false : (vsync == "1"); 
}

void GameData::Save() const
{
	std::ofstream file("assets/game_data.txt");

	file << "first_time: " << first_time_playing_ << "\n";

	file << "star_thresholds: ";
	for (auto& level : star_thresholds_) {
		file << level[0] << " " << level[1] << " ";
	}
	file << "\n";

	file << "custom_star_thresholds: ";
	for (auto& level : custom_star_thresholds_) {
		file << level[0] << " " << level[1] << " ";
	}
	file << "\n";

	file <<  "levels_completed: " << levels_completed_ << "\n";

	file << "record_times: ";
	for (auto time : record_times_) { 
		file << time << " ";
	}
	file << "\n";
	file << "custom_record_times: ";
	for (auto time : custom_record_times_) {
		file << time << " ";
	}
	file << "\n";

	file <<  "volume: " << volume_ << "\n";
	file <<  "muted: " << muted_ << "\n";
	file <<  "vsync: " << vsync_ << "\n";
}

