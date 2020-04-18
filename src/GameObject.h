#pragma once

#include "src/Aegis.h"

class GameObject
{
public:
	GameObject(float x, float y, float w, float h)
		:rect_{ x, y, w, h }
	{
		sprite_.pos_ = rect_.pos;
	}
		
	virtual void Update() {};
	virtual void Render(float delta_time) const = 0;
	virtual void SetPosition(Aegis::Vec2 pos);
	
	Aegis::Sprite sprite_;
	Aegis::AABB rect_;
	Aegis::Vec2 vel_;
	Aegis::Vec2 grid_coord_;

	float acceleration_ = 0.0f;
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
		sprite_.texture_ = Aegis::TextureManager::Instance().Load("assets/textures/tundra-tile-map.png");
		sprite_.size_ = { 32, 32 };
		sprite_.tex_coords_ = { 96.0f, 0.0f, 128.0f, 32.0f };
		sprite_.color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
		acceleration_ = 10.0f;
		anim_timer_.Stop();
	}
	void Update();
	void Render(float delta_time) const override;
	void StartMoving();

	float anim_time_per_tile_ = 0.15f;
	float total_anim_time_ = 0.0f;
	bool moving_ = false;
	Aegis::Vec2 target_pos_;
	Aegis::Vec2 dir_;
	Aegis::Timer anim_timer_;
	Aegis::Vec2 start_point_;
};

class Enemy : public GameObject
{
public:
	Enemy(int x, int y)
		:GameObject(x, y, 32, 32)
	{
		sprite_.texture_ = Aegis::TextureManager::Instance().Load("assets/textures/tundra-tile-map.png");
		sprite_.size_ = { 32, 32 };
		sprite_.tex_coords_ = { 128.0f, 0.0f, 160.0f, 32.0f };
		sprite_.color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
		acceleration_ = 1.0f;
	}

	void Update();
	void Render(float delta_time) const override;

	bool moving_ = true;
	Aegis::Vec2 target_pos_;
};