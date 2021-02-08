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
	virtual void Render(float delta_time) const {Aegis::RenderSprite(sprite_);}
	virtual void SetPosition(Aegis::Vec2 pos);
	virtual void MoveTo(const Aegis::Vec2 pos);
	virtual bool IsMoving() { return animation_.playing_;}
	
	std::shared_ptr<Aegis::Texture> spirte_sheet_;
	Animation animation_;
	Aegis::Sprite sprite_;
	Aegis::Vec2 grid_index_;
	Aegis::Vec2 target_grid_index_;
	//time to move 1 tile
	float speed_ = 0.15f;
	bool slides_on_ice_ = true;

};

class Pellet : public GameObject
{
public:
	Pellet(int x, int y)
		: GameObject(x, y, 16, 16, {144, 96})
	{}

	bool visible_;
};

class Player : public GameObject
{
public:
	Player(int x, int y)
		:GameObject(x,y,32,32, {0, 96}), queued_movement_(-1, 0)
	{
	}
	void Update() override;
	void MoveTo(const Aegis::Vec2 pos) override;

	Aegis::Vec2 queued_movement_;
};

class Brutus : public GameObject
{
public:
	Brutus(int x, int y)
		:GameObject(x, y, 32, 32, {32, 96})
	{
		speed_ = .30f;
		slides_on_ice_ = false;
	}

};

class Bjorn : public GameObject
{
public:
	Bjorn(int x, int y)
		:GameObject(x, y, 32, 32, {64, 96})
	{
		speed_ = .25f;

	}
};
