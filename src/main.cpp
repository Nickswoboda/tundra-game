#include "MenuScene.h"

int main()
{
	Aegis::Application app(1280, 720);
	app.PushScene(std::unique_ptr<Aegis::Scene>(new MenuScene()));
	app.Run();
}