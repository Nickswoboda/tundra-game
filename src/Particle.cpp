#include "Particle.h"

#include <random>

ParticleEngine::ParticleEngine(int num_particles, Aegis::Vec2 min_spawn_pos, Aegis::Vec2 max_spawn_pos, Aegis::Vec2 min_vel, Aegis::Vec2 max_vel)
	:x_pos_rng_(min_spawn_pos.x, max_spawn_pos.x), y_pos_rng_(min_spawn_pos.y, max_spawn_pos.y), 
	 x_vel_rng_(min_vel.x, max_vel.x), y_vel_rng_(min_vel.y, max_vel.y), num_particles_(num_particles)
{
	unsigned char white_data[4] = { 255, 255, 255, 255 };
	texture_ = Aegis::Texture::Create(white_data, 1, 1, 4);

	for (int i = 0; i < num_particles_; ++i){
		CreateParticle(i);
	}
}

void ParticleEngine::CreateParticle(int index)
{
	int size = index % 2 == 0 ? 4 : 5;
	particles_[index].Init(Aegis::Vec2(x_pos_rng_(rng_engine_), y_pos_rng_(rng_engine_)), Aegis::Vec2(x_vel_rng_(rng_engine_), y_vel_rng_(rng_engine_)), size);
}

void ParticleEngine::Update()
{
	static int window_height = Aegis::Application::GetWindow().GetSize().y;
	for (int i = 0; i < num_particles_; ++i){
		if (particles_[i].pos_.y > window_height) {
			CreateParticle(i);
		}
		else {
			particles_[i].pos_ += particles_[i].vel_;
		}
	}
}

void ParticleEngine::Render()
{
	for (int i = 0; i < num_particles_; ++i){
			Aegis::DrawQuad(particles_[i].pos_, Aegis::Vec2(particles_[i].size_, particles_[i].size_), *texture_, 1);
	}
}

void Particle::Init(Aegis::Vec2 pos, Aegis::Vec2 vel, int size)
{
	pos_ = pos;
	vel_ = vel;
	size_ = size;
}
