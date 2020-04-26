#include "Scenes/MenuScene.h"

int main()
{
	Aegis::Application app(1600, 900);
	app.ShowFrameTime(true);
	app.PushScene(std::unique_ptr<Aegis::Scene>(new MenuScene()));
	app.Run();
}