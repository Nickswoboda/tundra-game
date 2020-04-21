#pragma once

#include "src/Aegis.h"

#include <memory>
#include <unordered_map>
class FontManager
{
public:

	FontManager(FontManager const&) = delete;
	FontManager& operator=(FontManager const&) = delete;

	static FontManager& Instance()
	{
		static FontManager* instance = new FontManager();
		return *instance;
	}

	std::shared_ptr<Aegis::Font> Load(const std::string& path, int size)
	{
		if (fonts_.find(path) != fonts_.end()) {
			return fonts_[path];
		}

		std::shared_ptr<Aegis::Font> font = std::make_shared<Aegis::Font>(path, size);
		fonts_.emplace(path + std::to_string(size), font);

		return font;
	}
private:
	FontManager() {};
	std::unordered_map<std::string,std::shared_ptr<Aegis::Font>> fonts_;
};