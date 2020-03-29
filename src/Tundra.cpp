#include "src/Aegis.h"

#include <deque>
#include <iostream>

class Sandbox : public Aegis::Layer
{
public:
	Sandbox()
	{
	}

	void OnUpdate() override
	{
		Aegis::Renderer2D::Clear();
		Aegis::Renderer2D::DrawText("Hi", { 9, 0 }, { 1.0, 1.0, 0.0, 1.0 });
		Aegis::Renderer2D::DrawQuad({ 20, 20 }, { 300, 300 }, { 1.0, 1.0, 0.0, 1.0 });
	}
	void OnEvent(Aegis::Event& event)
	{
	}

};

int main()
{
	Aegis::Application app(640, 480);
	app.PushLayer(new Sandbox());
	app.Run();
}