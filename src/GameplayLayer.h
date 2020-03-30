#include "src/Aegis.h"
#include "Player.h"

class GameWorldLayer : public Aegis::Layer
{
public:
	GameWorldLayer()
	{
		Aegis::Application::SetVsync(true);
	}

	void OnUpdate() override
	{
		Aegis::Renderer2D::Clear();
		player_.OnUpdate();
		player_.OnRender();
	}
	void OnEvent(Aegis::Event& event)
	{
		player_.OnEvent(event);
	}

	Player player_;
};