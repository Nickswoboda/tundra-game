#include "EditorErrorDialog.h"

#include "../../Utilities.h"

EditorErrorDialog::EditorErrorDialog()
	:Aegis::Dialog("", {0, 0, 300, 200})
{
	auto temp_rect = rect_;
	Aegis::CenterAABB(temp_rect, Aegis::Application::GetWindow().GetViewport());
	SetPos(temp_rect.pos);
	
	AddButton("Okay", [&](){visible_ = false;});
	bg_color_ = k_bg_color;

	text_->SetFont(Aegis::FontManager::Load("assets/fonts/roboto_regular.ttf", 20));
	text_->SetText("Level is invalid");
	text_->color_ = {1.0f, 0.2f, 0.2f, 1.0f};
	temp_rect = text_->GetRect();
	Aegis::CenterAABBHorizontally(temp_rect, rect_);
	text_->SetPos(temp_rect.pos);

	error_label_ = std::make_shared<Aegis::Label>("", Aegis::Vec2(0, 0));
	error_label_->SetFont(Aegis::FontManager::Load("assets/fonts/roboto_regular.ttf", 18));
	error_label_->color_ = {1.0f, 0.2f, 0.2f, 1.0f};
	auto label_pos = temp_rect.pos + temp_rect.size + Aegis::Vec2(0, 5);
	error_label_->SetPos(label_pos);
}

void EditorErrorDialog::Render() const
{
	if (!visible_) return;

	DrawFrameBorder(rect_);
	Dialog::Render();
	error_label_->Render();
}

void EditorErrorDialog::Show(Error error)
{
	switch (error){
		case Error::PathFinding: 
			error_label_->SetText("Bears are unable to pathfind to Bruce.");
			break;
		case Error::FishReachability:
			error_label_->SetText("Bruce is unable to reach all fish.");
			break;
	}

	auto temp_rect = error_label_->GetRect();
	Aegis::CenterAABBHorizontally(temp_rect, rect_);
	error_label_->SetPos(temp_rect.pos);

	visible_ = true;
}

void EditorErrorDialog::AddButton(const std::string& label, std::function<void()> callback)
{
	auto button = button_container_->AddWidget<Aegis::Button>(Aegis::AABB(0,0, 100, 40), label);
	button->ConnectSignal("pressed", [&visible = visible_, callback]{visible = false; callback();});
	StylizeButton(*button, 2, 16);
}
