#include "LevelEditorScene.h"

LevelEditorScene::LevelEditorScene()
{
	tile_map_ = std::make_unique<TileMap>("assets/levels/level1.txt", 32); 
	//used to center tilemap within window
	camera_.SetPosition({-144, -24, 0});
	
	ui_layer_ = std::make_unique<Aegis::UILayer>();
	ui_layer_->SetFont(Aegis::FontManager::Instance().Load("assets/fonts/WorkSans-Regular.ttf", 32));
	
	back_button_ = ui_layer_->AddWidget<Aegis::Button>(new Aegis::Button({500, 500, 125, 40}, "BACK", [&](){ manager_->PopScene();}));  


}

LevelEditorScene::~LevelEditorScene()
{
}

void LevelEditorScene::OnEvent(Aegis::Event& event)
{

}

void LevelEditorScene::Update()
{

}

void LevelEditorScene::Render(float delta_time)
{
	Aegis::Renderer2D::SetProjection(camera_.view_projection_matrix_);
	Aegis::RendererClear();

	tile_map_->Render();
}
