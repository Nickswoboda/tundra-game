#include "GameData.h"

#include "src/Aegis.h"
#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>

GameData::GameData()
{
	std::string level_file = "assets/levels/level_" + std::to_string(num_levels_+1) + ".txt";
	while (std::filesystem::exists(level_file)) {
		++num_levels_;
		level_file = "assets/levels/level_" + std::to_string(num_levels_+1) + ".txt";
	}


	std::ifstream file("assets/levels/star_thresholds.txt");
	std::string thresholds;
	if (file) {
		file.seekg(0, std::ios::end);
		size_t size = file.tellg();
		if (size != -1) {
			thresholds.resize(size);
			file.seekg(0, std::ios::beg);
			file.read(&thresholds[0], size);
		}
	}

	int pos = 0; 
	while (pos < thresholds.size()){
		std::array<double, 2> level_thresholds;
		int first_num_end_pos = thresholds.find(',', pos);
		int second_num_end_pos = thresholds.find('\n', first_num_end_pos);
		if (first_num_end_pos == std::string::npos){
			std::cout << "Unable to read star thresholds. File may not be formatted correctly";
			return; 
		}
		auto first = thresholds.substr(pos, first_num_end_pos - pos);
		auto second = thresholds.substr(first_num_end_pos + 1, second_num_end_pos - first_num_end_pos + 1);
		level_thresholds[0] = stod(first);
		level_thresholds[1] = stod(second);
		star_thresholds_.push_back(level_thresholds);

		pos = second_num_end_pos + 1;
	}

};


