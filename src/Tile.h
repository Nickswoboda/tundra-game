#pragma once
#include <src/Aegis.h>

#include <memory>

class Tile
{
public:
	Tile() {};
	Tile(std::shared_ptr<Aegis::Texture> texture, Aegis::AABB subtex_rect, bool is_solid, bool is_slippery)
		: texture_(texture), size_({subtex_rect.size}), is_solid_(is_solid), is_slippery_(is_slippery)
	{
		float x1 = subtex_rect.pos.x / texture_->size_.x;
		float y1 = subtex_rect.pos.y / texture_->size_.y;
		float x2 = (subtex_rect.pos.x + subtex_rect.size.x) / texture_->size_.x;
		float y2 = (subtex_rect.pos.y + subtex_rect.size.y) / texture_->size_.y;
		texture_coords_ = { x1, y1, x2, y2 };
	}

	const std::shared_ptr<Aegis::Texture> texture_;
	Aegis::Vec4 texture_coords_;
	const Aegis::Vec2 size_;
	const bool is_solid_ = true; 
	const bool is_slippery_ = false;
	
};

