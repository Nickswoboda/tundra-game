#include "GameplayLayer.h"

int main()
{
	Aegis::Application app(640, 480);
	app.PushLayer(new GameplayLayer());
	app.Run();
}