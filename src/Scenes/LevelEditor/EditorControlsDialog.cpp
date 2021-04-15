#include "EditorControlsDialog.h"

#include "../../Utilities.h"

EditorControlsDialog::EditorControlsDialog()
	:Aegis::Dialog("",{0, 0, 310, 250})
{
	auto temp_rect = rect_;
	Aegis::CenterAABB(temp_rect, Aegis::Application::GetWindow().GetViewport());
	SetPos(temp_rect.pos);
	
	AddButton("Okay", [&](){visible_ = false;});
	bg_color_ = k_bg_color;

	auto font = Aegis::FontManager::Load("assets/fonts/roboto_regular.ttf", 18);

	text_box_ = std::make_shared<Aegis::Container>(Aegis::AABB{temp_rect.pos, {310, 200}}, Aegis::Container::Vertical, 10, Aegis::Alignment::Center);
	text_box_->AddWidget<Aegis::Label>("- Left-click to place ice tiles.", Aegis::Vec2{0,0});
	text_box_->AddWidget<Aegis::Label>("- Right-click to place wall tiles", Aegis::Vec2{0,0});
	text_box_->AddWidget<Aegis::Label>("- Press 'G' to place ground tiles", Aegis::Vec2{0,0});
	text_box_->AddWidget<Aegis::Label>("- Press 'F' to place fish on ice tiles. ", Aegis::Vec2{0,0});
	text_box_->AddWidget<Aegis::Label>("- Click and hold to drag Bruce/Bears.", Aegis::Vec2{0,0});
	
}

void EditorControlsDialog::Render() const
{
	if (!visible_) return;

	DrawFrameBorder(rect_);
	Dialog::Render();
	text_box_->Render();
}

void EditorControlsDialog::AddButton(const std::string& label, std::function<void()> callback)
{
	auto button = button_container_->AddWidget<Aegis::Button>(Aegis::AABB(0,0, 100, 40), label);
	button->ConnectSignal("pressed", [&visible = visible_, callback]{visible = false; callback();});
	StylizeButton(*button, 2, 16);
}
