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
	
	float current_frame = 0;
	float total_frames = 0;
	bool playing_ = false;

};

class GameObject
{
public:
	GameObject(float x, float y, float w, float h, Aegis::Vec2 subtex_pos = {0.0f, 0.0f})
		:sprite_({x,y}, {w, h}, Aegis::Texture::Create("assets/textures/tundra-tile-map.png"), subtex_pos)
	{
	}
		
	virtual void Update();
	virtual void Render(float delta_time) const = 0;
	virtual void SetPosition(Aegis::Vec2 pos);
	virtual void StartMoving();
	
	std::shared_ptr<Aegis::Texture> spirte_sheet_;
	Animation animation_;
	Aegis::Sprite sprite_;
	Aegis::Vec2 grid_coord_;
	//time to move 1 tile
	float speed_ = 0.15f;
	bool slides_on_ice_ = true;

};

class Pellet : public GameObject
{
public:
	Pellet(int x, int y)
		: GameObject(x, y, 8, 8), rect_(x,y,8,8)
	{}
	void Render(float delta_time) const override
	{
		Aegis::DrawQuad(rect_.pos, rect_.size, { 0.5, 0.5, 0.2, 1.0 });
	}

	Aegis::AABB rect_;
};

class Player : public GameObject
{
public:
	Player(int x, int y)
		:GameObject(x,y,32,32, {96, 0})
	{
	}
	void Render(float delta_time) const override;
};

class Brutus : public GameObject
{
public:
	Brutus(int x, int y)
		:GameObject(x, y, 32, 32, {128, 0})
	{
		speed_ = .30f;
		slides_on_ice_ = false;
	}
	void Render(float delta_time) const override;

};

class Bjorne : public GameObject
{
public:
	Bjorne(int x, int y)
		:GameObject(x, y, 32, 32, {160, 0})
	{
		speed_ = .25f;

	}
	void Render(float delta_time) const override;

};
