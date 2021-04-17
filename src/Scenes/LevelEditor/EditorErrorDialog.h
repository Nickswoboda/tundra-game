#pragma once

#include <src/Aegis.h>

#include <string>

enum class Error
{
	None,
	PathFinding,		//Bears unable to locate Bruce
	NoFish,
	FishReachability    //Bruce unable to reach all fish
};

class EditorErrorDialog : public Aegis::Dialog
{
public:
	EditorErrorDialog();

	void Render() const override;

	void Show(Error error);
private:
	void AddButton(const std::string& label, std::function<void()> callback) override;

	std::shared_ptr<Aegis::Label> error_label_;
};
