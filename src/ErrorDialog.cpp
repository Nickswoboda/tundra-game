#include "ErrorDialog.h"

#include "Utilities.h"

ErrorDialog::ErrorDialog()
	:Aegis::Dialog("", {0, 0, 350, 200})
{
	auto temp_rect = rect_;
	Aegis::CenterAABB(temp_rect, Aegis::Application::GetWindow().GetViewport());
	SetPos(temp_rect.pos);
	
	AddButton("Okay", [&](){visible_ = false;});
	bg_color_ = k_bg_color;

	text_->SetFont(Aegis::FontManager::Load("assets/fonts/roboto_regular.ttf", 20));
	text_->color_ = {1.0f, 0.2f, 0.2f, 1.0f};
}

void ErrorDialog::Render() const
{
	if (!visible_) return;

	DrawFrameBorder(rect_);
	Dialog::Render();
}

void ErrorDialog::Show(Error error)
{
	switch (error){
		case Error::PathFinding: 
			text_->SetText("Bears are unable to pathfind to Bruce.");
			break;
		case Error::NoFish:
			text_->SetText("There must be at least 1 fish.");
			break;
		case Error::FishReachability:
			text_->SetText("Bruce is unable to reach all fish.");
			break;
		case Error::TooManyCustomLevels:
			text_->SetText("You can only have 12 custom levels.");
			break;
	}

	auto temp_rect = text_->GetRect();
	Aegis::CenterAABBHorizontally(temp_rect, rect_);
	text_->SetPos(temp_rect.pos);

	visible_ = true;
}

void ErrorDialog::AddButton(const std::string& label, std::function<void()> callback)
{
	auto button = button_container_->AddWidget<Aegis::Button>(Aegis::AABB(0,0, 100, 40), label);
	button->ConnectSignal("pressed", [&visible = visible_, callback]{visible = false; callback();});
	StylizeButton(*button, 2, 16);
}
