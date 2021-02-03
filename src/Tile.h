#pragma once
#include <src/Aegis.h>

#include <memory>

class Tile
{
public:
	Tile() {};
	Tile(std::shared_ptr<Aegis::SubTexture> texture, bool is_solid, bool is_slippery)
		: texture_(texture), is_solid_(is_solid), is_slippery_(is_slippery)
	{}

	const std::shared_ptr<Aegis::SubTexture> texture_;
	const bool is_solid_ = true; 
	const bool is_slippery_ = false;
	
};

