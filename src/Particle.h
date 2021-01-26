#pragma once

#include "src/Aegis.h"

#include <array>
#include <random>

class Particle
{
public:
	void Init(Aegis::Vec2 pos, Aegis::Vec2 vel, int size);

	Aegis::Vec2 pos_;
	Aegis::Vec2 vel_;
	int size_;
};

class ParticleEngine
{
public:
	ParticleEngine(int num_particles, Aegis::Vec2 min_spawn_pos, Aegis::Vec2 max_spawn_pos, Aegis::Vec2 min_vel, Aegis::Vec2 max_vel);
	
	void CreateParticle(int index);
	void Update();
	void Render();

	std::array<Particle, 512> particles_;

	std::shared_ptr<Aegis::Texture> texture_;

	std::default_random_engine rng_engine_;

	std::uniform_real_distribution<float> x_pos_rng_;
	std::uniform_real_distribution<float> y_pos_rng_;
	std::uniform_real_distribution<float> x_vel_rng_;
	std::uniform_real_distribution<float> y_vel_rng_;


	int num_particles_;
};

