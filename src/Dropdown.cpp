#include "Dropdown.h"

#include "FontManager.h"

Dropdown::Dropdown(const std::string& label, Aegis::AABB rect)
{
	button_pos_offset_ = FontManager::Instance().Load("assets/fonts/WorkSans-Regular.ttf", 32)->GetStringPixelWidth(label) + 15;
	dropdown_button_ = new Aegis::Button({ rect.pos.x + button_pos_offset_, rect.pos.y, rect.size.x, rect.size.y });
	pos_ = rect.pos;
	size_ = rect.size;
	label_ = label;
}

Dropdown::~Dropdown()
{
	delete dropdown_button_;

	for (auto button : items_) {
		delete button.first;
	}
}

void Dropdown::Render()
{
	Aegis::DrawText(label_, pos_, { 1.0f, 1.0f, 1.0f, 1.0f });
	dropdown_button_->Render();

	if (collapsed_ && !items_.empty()) {
		items_[current_item_index].first->Render();
	}
	else {
		for (auto item : items_) {
			item.first->Render();
		}
	}
}

void Dropdown::AddItem(const std::string& text, std::function<void()> callback)
{
	float y_pos = pos_.y + size_.y * items_.size();

	Aegis::Button* temp_button = new Aegis::Button({ pos_.x + button_pos_offset_, y_pos, size_.x, size_.y }, text, FontManager::Instance().Load("assets/fonts/WorkSans-Regular.ttf", 32));

	items_.emplace_back(std::pair < Aegis::Button*, std::function<void()>>(temp_button, callback));
}

bool Dropdown::IsPressed(int action)
{
	if (collapsed_) {
		if (dropdown_button_->IsPressed(action)) {
			collapsed_ = false;
			return true;
		}
		return false;
	}
	
	for (int i = 0; i < items_.size(); ++i) {
		if (items_[i].first->IsPressed(action)) {
			current_item_index = i;
			items_[i].second();
			collapsed_ = true;
			MoveSelectedToTop();
			return true;
		}
	}

	return false;
}

void Dropdown::MoveSelectedToTop()
{
	items_[current_item_index].first->rect_ = { pos_.x + button_pos_offset_, pos_.y, size_.x, size_.y };

	int num_item = 1;
	for (int i = 0; i < items_.size(); ++i) {
		if (i != current_item_index) {
			float y_pos = pos_.y + size_.y * num_item;
			items_[i].first->rect_ = { pos_.x + button_pos_offset_, y_pos, size_.x, size_.y };
			++num_item;
		}
	}
}