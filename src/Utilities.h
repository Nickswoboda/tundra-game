#pragma once

#include "src/Aegis.h"
#include <string>

std::string FormatTime(double time_in_seconds);

void StylizeButton(Aegis::Button& button, int border_size, int font_size);
