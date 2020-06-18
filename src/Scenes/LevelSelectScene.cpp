#include "LevelSelectScene.h"

#include "GameplayScene.h"

LevelSelectScene::LevelSelectScene()
{
	title_font_ = Aegis::FontManager::Instance().Load("assets/fonts/WorkSans-Regular.ttf", 64);
	button_font_ = Aegis::FontManager::Instance().Load("assets/fonts/WorkSans-Regular.ttf", 32);

	back_button_ = new Aegis::Button({500, 650, 120, 30}, "Back", button_font_, [&]() { manager_->PopScene(); });
	select_button_ = new Aegis::Button({ 640, 650, 120, 30 }, "Select", button_font_, [&]() { manager_->PushScene(std::unique_ptr<Scene>(new GameplayScene(selected_level_))); });

	int level = 0;
	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col){
			level_buttons_.emplace_back(new Aegis::Button({ 500 + (float)(col * 74), 200 + (float)(row * 74), 64, 64 }, std::to_string((row * 4) + col + 1), button_font_,
				[&, level]() { selected_level_ = level; }));
			++level;
		}
	}
}

LevelSelectScene::~LevelSelectScene()
{
	delete back_button_;
	delete select_button_;
	
	for (auto& button : level_buttons_) {
		delete button;
	}
}
void LevelSelectScene::Update()
{

}

void LevelSelectScene::Render(float delta_time)
{
	Aegis::Renderer2D::SetProjection(camera_.view_projection_matrix_);
	Aegis::RendererClear();

	Aegis::Renderer2D::SetFont(title_font_);
	Aegis::DrawText("Level Select", { 490, 20 }, { 1.0, 1.0f, 1.0f, 1.0f });


	back_button_->Render();
	select_button_->Render();
	
	for (const auto& button : level_buttons_) {
		button->Render();
	}
	if (selected_level_ != -1) {
		auto pos = level_buttons_[selected_level_]->rect_.pos;
		auto size = level_buttons_[selected_level_]->rect_.size;
		Aegis::DrawQuad(pos, size, { 1.0f, 1.0f, 1.0f, 0.4 });
	}
}

void LevelSelectScene::OnEvent(Aegis::Event& event)
{
}
