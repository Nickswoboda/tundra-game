#pragma once

#include "src/Aegis.h"

class GameplayScene;
class ScoreDialog : public Aegis::Dialog
{
public:
	ScoreDialog(GameplayScene& scene);

	void Render() const override;
	void OnEvent(Aegis::Event& event) override;

	void Show(double time);

private:
	void AddButton(const std::string& label, std::function<void()> callback) override;
	int GetNumStarsEarned(double time_in_seconds) const;

	std::unique_ptr<Aegis::Container> score_container_;
	std::shared_ptr<Aegis::Label> time_label_;
	std::array<std::shared_ptr<Aegis::SpriteWidget>, 3> star_sprites_;
	const std::array<double, 2>& star_thresholds_;
};
