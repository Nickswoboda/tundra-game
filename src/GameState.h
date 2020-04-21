#pragma once 

#include "src/Aegis.h"
class GameState
{
public:
	virtual void Update() = 0;
	virtual void Render(float delta_time) = 0;
	virtual void OnEvent(Aegis::Event& event) = 0;

};