#include "src/Aegis.h"
#include "Player.h"

class Wall
{
public:
	void OnRender() const
	{
		Aegis::Renderer2D::DrawQuad({ x_pos_, y_pos_ }, { width_, height_ }, { 0.2, 0.2, 0.2, 1.0 });
	}
	
	float x_pos_ = 0.0f;
	float y_pos_ = 0.0f;

	int width_ = 32;
	int height_ = 32;
	
};
class GameWorldLayer : public Aegis::Layer
{
public:
	GameWorldLayer()
	{
		//Aegis::Application::SetVsync(true);
		for (int i = 0; i < 20; ++i) {
			walls_[i].x_pos_ = i * 32;
		}
		for (int i = 20; i < 40; ++i) {
			walls_[i].x_pos_ = (i - 20) * 32;
			walls_[i].y_pos_ = 480 - 32;
		}
		for (int i = 40; i < 60; ++i) {
			walls_[i].x_pos_ = 640 - 32;
			walls_[i].y_pos_ = (i - 40) * 32;
		}
		for (int i = 60; i < 80; ++i) {
			walls_[i].y_pos_ = (i - 60) * 32;
		}

	}
	void CheckCollision(const Player& player, const Wall& wall) 
	{
		if (player.x_pos_ < wall.x_pos_ + wall.width_ &&
			player.x_pos_ + player.width_ > wall.x_pos_&&
			player.y_pos_ < wall.y_pos_ + wall.height_ &&
			player.y_pos_ + player.height_ > wall.y_pos_) {

			player_.x_pos_ -=	player.x_vel_;
			player_.y_pos_ -=	player.y_vel_;
			player_.x_vel_ = 0;
			player_.y_vel_ = 0;

		}

	}

	void OnUpdate() override
	{
		Aegis::Renderer2D::Clear();
		player_.OnUpdate();
		player_.OnRender();
		
		for (const auto& wall : walls_) {
			wall.OnRender();
			CheckCollision(player_, wall);
		}

		Aegis::Renderer2D::DrawText(std::to_string(Aegis::Application::GetFrameTime()), { 0, 0 }, { 1.0f, 1.0f, 1.0f, 1.0f });
	}
	void OnEvent(Aegis::Event& event)
	{
		player_.OnEvent(event);
	}

	Player player_;
	Wall walls_[80];
};