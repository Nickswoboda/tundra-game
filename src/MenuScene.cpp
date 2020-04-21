#include "MenuScene.h"

MenuScene::MenuScene()
{
}

void MenuScene::Update()
{
}

void MenuScene::Render(float delta_time)
{
	Aegis::RendererClear();
	Aegis::DrawText("YOU LOSE", { 640, 360 }, { 1.0, 0.0, 0.0, 1.0f });
	Aegis::DrawText("Press Enter to Restart", { 600, 380 }, { 1.0, 1.0, 0.0, 1.0f });
}

void MenuScene::OnEvent(Aegis::Event& event)
{
	auto key_event = dynamic_cast<Aegis::KeyEvent*>(&event);
	if (key_event) {
		if (key_event->key_ == AE_KEY_ENTER && key_event->action_ == AE_BUTTON_PRESS) {
			manager_->PopScene();
		}
	}
}
