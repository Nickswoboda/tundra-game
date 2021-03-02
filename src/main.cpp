#include "Scenes/MenuScene.h"
#include "GameData.h"
int main()
{
	Aegis::Application::CreateWindow("Tundra", 1280, 720, Aegis::WindowFlag_Centered);
	Aegis::Application::ShowFrameTime(true);
	GameData game_data_;
	Aegis::Application::Start(std::make_unique<MenuScene>(game_data_));
}
