#include "src/Aegis.h"

#include <glm/glm.hpp>

class GameObject
{
public:
	GameObject(float x, float y, int w, int h)
		: x_pos_(x), y_pos_(y), width_(w), height_(h)
	{}
		
	virtual void OnUpdate() {};
	virtual void OnRender(float delta_time) const = 0;
				 
	float x_pos_;
	float y_pos_;
				
	int width_;
	int height_;

	float x_vel_ = 0.0f;
	float y_vel_ = 0.0f;

	float acceleration_ = 0.0f;

	glm::vec4 color_ = { 1.0, 1.0, 1.0, 1.0 };
};

class Pellet : public GameObject
{
public:
	Pellet()
		: GameObject(0, 0, 16, 16)
	{}
	void OnRender(float delta_time) const override
	{
		Aegis::Renderer2D::DrawQuad({ x_pos_, y_pos_ }, { width_, height_ }, { 0.5, 0.8, 0.2, 1.0 });
	}
};
class Wall : public GameObject
{
public:
	Wall()
		: GameObject(0, 0, 32, 32)
	{}
	void OnRender(float delta_time) const override
	{
		Aegis::Renderer2D::DrawQuad({ x_pos_, y_pos_ }, { width_, height_ }, { 0.2, 0.2, 0.2, 1.0 });
	}
};

class Player : public GameObject
{
public:
	Player()
		:GameObject(0,0,32,32)
	{
		acceleration_ = 10.0f;
	}

	void OnUpdate();
	void OnEvent(Aegis::Event& event);
	void OnRender(float delta_time) const override;
};