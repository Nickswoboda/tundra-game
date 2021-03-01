#include "Scenes/MenuScene.h"
#include "Scenes/OptionsScene.h"

int main()
{
	Aegis::Application::CreateWindow("Tundra", 1280, 720, Aegis::WindowFlag_Centered);
	Aegis::Application::ShowFrameTime(true);
	Aegis::Application::Start(std::make_unique<MenuScene>());
}