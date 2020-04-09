#include "GameplayLayer.h"

int main()
{
	Aegis::Application app(1280, 720);
	app.PushLayer(new GameplayLayer());
	app.Run();
}