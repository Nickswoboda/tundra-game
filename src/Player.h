#include "src/Aegis.h"

class Player
{
public:
	void OnUpdate();
	void OnEvent(Aegis::Event& event);
	void OnRender();

	float x_pos_ = 0.0f;
	float y_pos_ = 0.0f;

	int width_ = 32;
	int height_ = 32;

	float x_vel_ = 0.0f;
	float y_vel_ = 0.0f;

	float acceleration_ = 1.0f;

};