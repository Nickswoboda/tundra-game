#pragma once

#include <src/Aegis.h>

class InfoDialog : public Aegis::Widget
{
public:
	InfoDialog();

	void Render() const override;
	void OnEvent(Aegis::Event& event) override;

private:
	std::shared_ptr<Aegis::Button> close_button_;
	std::shared_ptr<Aegis::Texture> bg_;
	Aegis::VContainer v_box_;
};
