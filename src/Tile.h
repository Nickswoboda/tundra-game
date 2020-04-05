#pragma once

#include <src/Aegis.h>

struct Tile
{
public:
	glm::vec4 color_;
	glm::vec2 pos_;
	glm::vec2 size_;
	bool solid_ = false;
	bool slippery_ = false;
};

struct Wall : public Tile
{
	Wall(int x, int y)
	{
		pos_ = { x, y };
		color_ = { 0.2f, 0.2f, 0.2f, 1.0f };
		size_ = { 32, 32 };
		solid_ = true;
	}
};

struct Ground : public Tile
{
	Ground(int x, int y)
	{
		pos_ = { x, y };
		color_ = { 0.95f, 0.95f, 0.95f, 1.0f };
		size_ = { 32, 32 };
	}
};

struct Ice : public Tile
{
	Ice(int x, int y)
	{
		pos_ = { x, y };
		color_ = { 0.84f, 1.0f, .99f, 1.0f };
		size_ = { 32, 32 };
		slippery_ = true;
	}
};
