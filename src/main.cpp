#include "Scenes/MenuScene.h"
#include "Scenes/OptionsScene.h"

int main()
{
	Aegis::Application::CreateWindow("Tundra", 1280, 720);
	Aegis::Application::ShowFrameTime(true);
	Aegis::Application::PushScene(std::unique_ptr<Aegis::Scene>(new MenuScene()));
	Aegis::Application::Run();
}