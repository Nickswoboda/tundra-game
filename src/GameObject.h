#pragma once

#include "src/Aegis.h"

class Animation
{
public:
	Animation(Aegis::Sprite& sprite, const std::string& sfx_path = "")
		: sprite_(sprite)
	{
	}
	void Start(Aegis::Vec2 end, float speed, int num_tiles);
	void Update();
	void Stop();
	void SetSfx(const std::string& path)
	{
		sfx_ = std::make_shared<Aegis::SoundEffect>(path);
	}

	Aegis::Sprite& sprite_;
	std::shared_ptr<Aegis::SoundEffect> sfx_;

	Aegis::Vec2 start_value_;
	Aegis::Vec2 current_value_;
	Aegis::Vec2 end_value_;
	
	//bears should rotate twice every tile to simulate movement.
	float percent_to_rotate_at_ = 0;
	int current_rotations_ = 0;
	int current_frame_ = 0;
	int total_frames_ = 0;
	bool playing_ = false;
};

class GameObject
{
public:
	GameObject(float x, float y, float w, float h, Aegis::AABB subtex_rect)
		:sprite_(Aegis::Texture::Create("assets/textures/tundra-tile-map.png"), subtex_rect)
	{
		sprite_.position_ = { x, y };
	}
		
	virtual void Update();
	virtual void Render(float delta_time) const { sprite_.Draw(); }
	virtual void SetPosition(Aegis::Vec2 pos);
	virtual void MoveTo(const Aegis::Vec2 pos);
	virtual bool IsMoving() { return animation_.playing_;}

	Aegis::AABB GetRect() const;
	
	Aegis::Sprite sprite_;
	Animation animation_ = Animation(sprite_);
	Aegis::Vec2 grid_index_;
	Aegis::Vec2 target_grid_index_;
	//time to move 1 tile
	float speed_ = 0.20f;
	bool slides_on_ice_ = true;

};

class Pellet : public GameObject
{
public:
	Pellet(int x, int y)
		: GameObject(x, y, 16, 16, { 96, 96, 32, 32 })
	{
		sprite_.scale_ = { 0.5f, 0.5f };
	}

	bool visible_ = true;
};

class Player : public GameObject
{
public:
	Player(int x, int y)
		:GameObject(x,y,32,32, {0, 96, 32, 32}), queued_movement_(-1, 0)
	{
		animation_.SetSfx("assets/audio/bruce-footsteps.ogg");
	}
	void Update() override;
	void MoveTo(const Aegis::Vec2 pos) override;

	Aegis::Vec2 queued_movement_;
};

class Brutus : public GameObject
{
public:
	Brutus(int x, int y)
		:GameObject(x, y, 32, 32, {32, 96, 32, 32})
	{
		speed_ = .30f;
		slides_on_ice_ = false;
		animation_.SetSfx("assets/audio/brutus-footsteps.ogg"); 
	}
};

class Bjorn : public GameObject
{
public:
	Bjorn(int x, int y)
		:GameObject(x, y, 32, 32, {64, 96, 32, 32})
	{
		speed_ = .25f;
	}
};
