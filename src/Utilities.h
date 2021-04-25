#pragma once

#include "src/Aegis.h"
#include <string>

std::string FormatTime(double time_in_seconds);

void StylizeButton(Aegis::Button& button, int border_size, int font_size);
void StylizeSpinBox(Aegis::SpinBox& box, int border_size, int font_size);

void DrawFrameBorder(const Aegis::AABB& rect);

const Aegis::Vec4 k_bg_color{ 0.22f, 0.31f, 0.42f, 1.0f };
const Aegis::Vec4 k_button_color{ 0.37f, 0.5f,  0.72f, 1.0f };
const Aegis::Vec4 k_button_hover_color{ 0.45f, 0.58f, 0.78f, 1.0f };
