#pragma once

#include "src/Aegis.h"
class Animation
{
public:
	void Start(Aegis::Vec2 start, Aegis::Vec2 end, float duration);
	void Update();
	void Stop();
	Aegis::Vec2 start_value_;
	Aegis::Vec2 current_value_;
	Aegis::Vec2 end_value_;
	Aegis::Timer timer_;
	float duration_ = 0.0f;
	bool playing_ = false;

};

class GameObject
{
public:
	GameObject(float x, float y, float w, float h)
		:rect_{ x, y, w, h }
	{
		sprite_.pos_ = rect_.pos;
		sprite_.texture_ = Aegis::TextureManager::Instance().Load("assets/textures/tundra-tile-map.png");
		sprite_.size_ = { 32, 32 };
		sprite_.color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	}
		
	virtual void Update();
	virtual void Render(float delta_time) const = 0;
	virtual void SetPosition(Aegis::Vec2 pos);
	virtual void StartMoving();
	
	Animation animation_;
	Aegis::Sprite sprite_;
	Aegis::AABB rect_;
	Aegis::Vec2 grid_coord_;
	//time to move 1 tile
	float speed_ = 0.15f;

};

class Pellet : public GameObject
{
public:
	Pellet(int x, int y)
		: GameObject(x, y, 8, 8)
	{}
	void Render(float delta_time) const override
	{
		Aegis::DrawQuad(rect_.pos, rect_.size, { 0.5, 0.5, 0.2, 1.0 });
	}
};

class Player : public GameObject
{
public:
	Player(int x, int y)
		:GameObject(x,y,32,32)
	{
		sprite_.tex_coords_ = { 96.0f, 0.0f, 128.0f, 32.0f };
	}
	void Render(float delta_time) const override;
};

class Brutus : public GameObject
{
public:
	Brutus(int x, int y)
		:GameObject(x, y, 32, 32)
	{
		sprite_.tex_coords_ = { 128.0f, 0.0f, 160.0f, 32.0f };
		speed_ = .25f;

	}
	void Render(float delta_time) const override;

};

class Bjorne : public GameObject
{
public:
	Bjorne(int x, int y)
		:GameObject(x, y, 32, 32)
	{
		sprite_.tex_coords_ = { 160.0f, 0.0f, 192.0f, 32.0f };
		speed_ = .20f;

	}
	void Render(float delta_time) const override;

};