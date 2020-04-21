#pragma once

#include "src/Aegis.h"
class Button
{
public:
	Button(Aegis::AABB rect);
	Button(Aegis::AABB rect, std::string text, std::shared_ptr<Aegis::Font> font);

	void Render();

private:
	Aegis::AABB rect_;
	Aegis::Vec4 color_ = { 0.2, 0.4, 0.9, 1.0 };
	std::string text_;
	std::shared_ptr<Aegis::Font> font_;
	
};