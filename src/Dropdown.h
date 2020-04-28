#pragma once

#include "src/Aegis.h"

#include <string>
#include <functional>
#include <queue>

class Dropdown
{
public:
	Dropdown(Aegis::AABB rect);
	~Dropdown();

	void Render();
	void AddItem(const std::string& text, std::function<void()> callback);
	bool IsPressed(int action);
	void MoveSelectedToTop();
	

	Aegis::Button* dropdown_button_ = nullptr;
	std::vector<std::pair<Aegis::Button*, std::function<void()>>> items_;

	Aegis::Vec2 pos_;
	Aegis::Vec2 size_;
	int current_item_index = 0;
	bool collapsed_ = true;
};