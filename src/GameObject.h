#pragma once

#include "src/Aegis.h"

class GameObject
{
public:
	GameObject(float x, float y, float w, float h)
		: pos_{ x, y }, size_{w, h}
	{}
		
	virtual void Update() {};
	virtual void Render(float delta_time) const = 0;
				 
	Aegis::Vec2 pos_;
	Aegis::Vec2 size_;

	float x_vel_ = 0.0f;
	float y_vel_ = 0.0f;

	float acceleration_ = 0.0f;

	Aegis::Vec4 color_ = { 1.0, 1.0, 1.0, 1.0 };

	bool destructible_ = false;
};

class Pellet : public GameObject
{
public:
	Pellet(int x, int y)
		: GameObject(x, y, 16, 16)
	{
		destructible_ = true;
	}
	void Render(float delta_time) const override
	{
		Aegis::Renderer2D::DrawQuad(pos_, size_, { 0.5, 0.8, 0.2, 1.0 });
	}
};

class Player : public GameObject
{
public:
	Player(int x, int y)
		:GameObject(x,y,32,32)
	{
		acceleration_ = 10.0f;
	}

	void Update();
	void OnEvent(Aegis::Event& event);
	void Render(float delta_time) const override;
};