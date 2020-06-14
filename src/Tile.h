#pragma once

#include <src/Aegis.h>

struct Tile
{
	enum Type {
		Wall,
		Ice,
		Ground,
		NumTypes
	};
public:
	Type type_ = Type::NumTypes;
	Aegis::Vec2 pos_;
	bool is_solid_ = false;
	bool is_slippery_ = false;
};

struct Wall : public Tile
{
	Wall(int x, int y)
	{
		type_ = Type::Wall;
		pos_.x = x;
		pos_.y = y;
		is_solid_ = true;
	}
};

struct Ground : public Tile
{
	Ground(int x, int y)
	{
		type_ = Type::Ground;
		pos_.x = x;
		pos_.y = y;
	}
};

struct Ice : public Tile
{
	Ice(int x, int y)
	{
		type_ = Type::Ice;
		pos_.x = x;
		pos_.y = y;
		is_slippery_ = true;
	}
};
