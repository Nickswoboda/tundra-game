#include "GameOverDialog.h"

#include "Scenes/GameplayScene.h"
#include "Utilities.h"


GameOverDialog::GameOverDialog(GameplayScene& scene)
	:Aegis::Dialog("You lose. Try Again?", {0,0,300, 200})
{
	Aegis::AABB temp_rect = {0,0, 300, 200};
	Aegis::CenterAABB(temp_rect, Aegis::Application::GetWindow().GetViewport());
	SetPos(temp_rect.pos);

	AddButton("Retry", [&]() {scene.SetUpLevel(); });
	AddButton("Menu", [&]() {scene.manager_->PopScene(); });

	bg_color_ =  k_bg_color;

	text_->SetFont(Aegis::FontManager::Load("assets/fonts/roboto_regular.ttf", 28));
	auto text_rect = Aegis::AABB({ 0, 0 }, text_->font_->GetStringPixelSize(text_->GetText()));
	Aegis::CenterAABBHorizontally(text_rect, rect_);
	text_->SetPos({text_rect.pos.x, rect_.pos.y + rect_.size.y / 3});
};

void GameOverDialog::AddButton(const std::string& label, std::function<void()> callback)
{
	auto button = button_container_->AddWidget<Aegis::Button>(Aegis::AABB(0,0, 100, 40), label);
	button->ConnectSignal("pressed", [&visible = visible_, callback]{visible = false; callback();});
	StylizeButton(*button, 2, 24);
}

void GameOverDialog::Render() const 
{
	DrawFrameBorder(rect_);
	Dialog::Render();
}
