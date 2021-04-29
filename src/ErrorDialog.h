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

class ErrorDialog : public Aegis::Dialog
{
public:
	ErrorDialog();

	void Render() const override;

	void Show(Error error);
private:
	void AddButton(const std::string& label, std::function<void()> callback) override;
};
