#pragma once

#include "src/Aegis.h"

class ScoreBoard : public Aegis::Widget
{
public:
	ScoreBoard(int num_lives, int num_pellets);

	void Render() const override;
	void OnEvent(Aegis::Event& event) override;

	void SetTimeLabel(const std::string& label);
	void SetNumLives(int num_lives);
	void SetPelletCount(int current, int total);

	std::array<std::shared_ptr<Aegis::SpriteWidget>, 3> heart_widgets_;
	std::shared_ptr<Aegis::Label> pellet_count_label_;
	std::shared_ptr<Aegis::Label> time_label_;

	std::unique_ptr<Aegis::Container> v_box_;
	std::unique_ptr<Aegis::SpriteWidget> bg_;
};
