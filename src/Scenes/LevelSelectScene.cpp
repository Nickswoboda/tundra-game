#include "LevelSelectScene.h"

#include "GameplayScene.h"
#include "LevelEditorScene.h"

#include <filesystem>
LevelSelectScene::LevelSelectScene()
{
	title_font_ = Aegis::FontManager::Load("assets/fonts/WorkSans-Regular.ttf", 64);
	button_font_ = Aegis::FontManager::Load("assets/fonts/WorkSans-Regular.ttf", 32);

	ui_layer_ = std::make_unique<Aegis::UILayer>();
	ui_layer_->SetFont(button_font_);
	auto back_button_ = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({500, 650, 120, 30}, "Back", [&]() { manager_->PopScene(); }));
	auto select_button_ = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({ 640, 650, 120, 30 }, "Select", [&]() { manager_->PushScene(std::unique_ptr<Scene>(new GameplayScene(selected_level_))); }));

	int level = 1;
	int x_pos = 500;
	int y_pos = 200;
	while (std::filesystem::exists("assets/levels/level" + std::to_string(level) + ".txt")){
		level_buttons_.emplace_back(ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({(float)x_pos, (float)y_pos, 64, 64}, std::to_string(level), [&, level](){selected_level_ = level;})));
		x_pos += 74;

		//maximum of 4 buttons per row
		if (level % 4 == 0){
			x_pos = 500;
			y_pos += 74;
		}

		++level;
	}
}

LevelSelectScene::~LevelSelectScene()
{
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

	if (selected_level_ != -1) {
		auto pos = level_buttons_[selected_level_ - 1]->rect_.pos;
		auto size = level_buttons_[selected_level_ - 1]->rect_.size;
		Aegis::DrawQuad(pos, size, { 1.0f, 1.0f, 1.0f, 0.4 });
	}
}

void LevelSelectScene::OnEvent(Aegis::Event& event)
{
}
