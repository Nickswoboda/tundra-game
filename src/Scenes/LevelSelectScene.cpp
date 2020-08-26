#include "LevelSelectScene.h"

#include "GameplayScene.h"
#include "LevelEditorScene.h"

LevelSelectScene::LevelSelectScene()
{
	title_font_ = Aegis::FontManager::Instance().Load("assets/fonts/WorkSans-Regular.ttf", 64);
	button_font_ = Aegis::FontManager::Instance().Load("assets/fonts/WorkSans-Regular.ttf", 32);

	ui_layer_ = std::make_unique<Aegis::UILayer>();
	ui_layer_->SetFont(button_font_);
	back_button_ = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({500, 650, 120, 30}, "Back", [&]() { manager_->PopScene(); }));
	select_button_ = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({ 640, 650, 120, 30 }, "Select", [&]() { manager_->PushScene(std::unique_ptr<Scene>(new GameplayScene(selected_level_))); }));
	editor_button_ = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({ 780, 650, 120, 30 }, "Editor", [&]() { manager_->PushScene(std::unique_ptr<Scene>(new LevelEditorScene())); }));
	

	int level = 0;
	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col){
			level_buttons_.emplace_back(ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({ 500 + (float)(col * 74), 200 + (float)(row * 74), 64, 64 }, std::to_string((row * 4) + col + 1), 
				[&, level]() { selected_level_ = level; })));
			++level;
		}
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
		auto pos = level_buttons_[selected_level_]->rect_.pos;
		auto size = level_buttons_[selected_level_]->rect_.size;
		Aegis::DrawQuad(pos, size, { 1.0f, 1.0f, 1.0f, 0.4 });
	}
}

void LevelSelectScene::OnEvent(Aegis::Event& event)
{
}
