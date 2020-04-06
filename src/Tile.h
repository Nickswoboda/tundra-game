#pragma once

#include <src/Aegis.h>

struct Tile
{
public:
	glm::vec4 color_;
	bool is_solid_ = false;
	bool is_slippery_ = false;
};

struct Wall : public Tile
{
	Wall()
	{
		color_ = { 0.2f, 0.2f, 0.2f, 1.0f };
		is_solid_ = true;
	}
};

struct Ground : public Tile
{
	Ground()
	{
		color_ = { 0.95f, 0.95f, 0.95f, 1.0f };
	}
};

struct Ice : public Tile
{
	Ice()
	{
		color_ = { 0.84f, 1.0f, .99f, 1.0f };
		is_slippery_ = true;
	}
};
