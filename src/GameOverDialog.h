#pragma once 

#include "src/Aegis.h"

class GameplayScene;

class GameOverDialog : public Aegis::Dialog
{
public:
	GameOverDialog(GameplayScene& scene);

	void Render() const override;
	void Show();
private:
	void AddButton(const std::string& label, std::function<void()> callback) override;
};
