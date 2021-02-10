#pragma once
#include <src/Aegis.h>

#include <memory>

class Tile
{
public:
	Tile() {};
	Tile(Aegis::Sprite sprite, bool is_solid, bool is_slippery)
		: sprite_(sprite), is_solid_(is_solid), is_slippery_(is_slippery)
	{}

	const Aegis::Sprite sprite_;
	const bool is_solid_ = true; 
	const bool is_slippery_ = false;
	
};

