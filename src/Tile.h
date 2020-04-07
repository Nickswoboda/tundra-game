#pragma once

#include <src/Aegis.h>

struct Tile
{
public:
	Aegis::Vec4 color_;
	Aegis::Vec2 pos_;
	bool is_solid_ = false;
	bool is_slippery_ = false;
};

struct Wall : public Tile
{
	Wall(int x, int y)
	{
		pos_.x = x;
		pos_.y = y;
		color_ = { 0.2f, 0.2f, 0.2f, 1.0f };
		is_solid_ = true;
	}
};

struct Ground : public Tile
{
	Ground(int x, int y)
	{
		pos_.x = x;
		pos_.y = y;
		color_ = { 0.95f, 0.95f, 0.95f, 1.0f };
	}
};

struct Ice : public Tile
{
	Ice(int x, int y)
	{
		pos_.x = x;
		pos_.y = y;
		color_ = { 0.84f, 1.0f, .99f, 1.0f };
		is_slippery_ = true;
	}
};
