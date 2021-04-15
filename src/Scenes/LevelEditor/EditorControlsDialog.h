#pragma once

#include <src/Aegis.h>

class EditorControlsDialog : public Aegis::Dialog
{
public:
	EditorControlsDialog();

	void Render() const override;

	void AddButton(const std::string& label, std::function<void()> callback) override;

	std::shared_ptr<Aegis::Container> text_box_;
};
