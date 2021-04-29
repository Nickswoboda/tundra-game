#pragma once

#include <src/Aegis.h>

#include <string>

enum class Error
{
	None,
	PathFinding,		//Bears unable to locate Bruce
	NoFish,
	FishReachability,   //Bruce unable to reach all fish
	TooManyCustomLevels,
	InvalidTiles
};

class PopUpDialog : public Aegis::Dialog
{
public:
	PopUpDialog();

	void Render() const override;

	void Show(Error error);
	void Show(const std::string& text);

private:
	void AddButton(const std::string& label, std::function<void()> callback) override;
};
