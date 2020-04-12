#pragma once

#include "src/Aegis.h"

class GameObject
{
public:
	GameObject(float x, float y, float w, float h)
		:rect_{ x, y, w, h }
	{}
		
	virtual void Update() {};
	virtual void Render(float delta_time) const = 0;
				 
	Aegis::Vec4 uv_coords_;
	Aegis::AABB rect_;
	Aegis::Vec2 vel_;
	Aegis::Vec2 tile_index_;
	std::unique_ptr<Aegis::Texture> texture_;

	float acceleration_ = 0.0f;

	bool destructible_ = false;
};

class Pellet : public GameObject
{
public:
	Pellet(int x, int y)
		: GameObject(x, y, 8, 8)
	{
		destructible_ = true;
	}
	void Render(float delta_time) const override
	{
		Aegis::DrawQuad(rect_.pos, rect_.size, { 0.5, 0.5, 0.2, 1.0 });
	}
};

class Player : public GameObject
{
public:
	Player(int x, int y)
		:GameObject(x,y,20,20)
	{
		acceleration_ = 10.0f;
		tile_index_ = { -1, -1 };
		uv_coords_ = { 96.0f, 0.0f, 128.0f, 32.0f };
	}

	void Update();
	void OnEvent(Aegis::Event& event);
	void Render(float delta_time) const override;
};