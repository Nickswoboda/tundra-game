#pragma once

#include <src/Aegis.h>

struct Tile
{
	enum class Type {
		Wall,
		Ice,
		Ground,
		None
	};
public:
	Type type_ = Type::None;
	Aegis::Vec4 color_;
	Aegis::Vec2 pos_;
	Aegis::Vec4 uv_coords_;
	bool is_solid_ = false;
	bool is_slippery_ = false;
};

struct Wall : public Tile
{
	Wall(int x, int y)
	{
		uv_coords_ = { 0.0f, 0.0f, 32.0f, 32.0f };
		type_ = Type::Wall;
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
		uv_coords_ = { 32.0f, 0.0f, 64.0f, 32.0f };
		type_ = Type::Ground;
		pos_.x = x;
		pos_.y = y;
		color_ = { 0.95f, 0.95f, 0.95f, 1.0f };
	}
};

struct Ice : public Tile
{
	Ice(int x, int y)
	{
		uv_coords_ = { 64.0f, 0.0f, 96.0f, 32.0f };
		type_ = Type::Ice;
		pos_.x = x;
		pos_.y = y;
		color_ = { 0.84f, 1.0f, .99f, 1.0f };
		is_slippery_ = true;
	}
};
