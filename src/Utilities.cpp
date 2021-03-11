#include "Utilities.h"

std::string FormatTime(double time_in_seconds)
{
	int num_mins = time_in_seconds / 60;
	time_in_seconds -= num_mins * 60;

	int num_secs = time_in_seconds;
	time_in_seconds -= num_secs;

	int num_cs = time_in_seconds * 100;

	auto mins = num_mins >= 10 ? std::to_string(num_mins) : "0" + std::to_string(num_mins);
	auto secs = num_secs >= 10 ? std::to_string(num_secs) : "0" + std::to_string(num_secs);
	auto cs = num_cs >= 10 ? std::to_string(num_cs) : "0" + std::to_string(num_cs);

	return mins + ":" + secs + ":" + cs;
}

void StylizeButton(Aegis::Button& button, int border_size, int font_size)
{
	button.SetStateBgColor(Aegis::Button::Normal, { 0.37f, 0.5f,  0.72f, 1.0f });
	button.SetStateBgColor(Aegis::Button::Pressed, { 0.37f, 0.5f,  0.72f, 1.0f });
	button.SetStateBgColor(Aegis::Button::Hovered, { 0.45f, 0.58f, 0.78f, 1.0f });
	button.border_size_ = border_size;

	button.SetFont(Aegis::FontManager::Load("assets/fonts/roboto_bold.ttf", font_size));

	auto hover_sfx = Aegis::AudioPlayer::LoadSound("assets/audio/button_hover.ogg");
	button.ConnectSignal("entered", [hover_sfx]() {Aegis::AudioPlayer::PlaySound(hover_sfx, 40); });
}

