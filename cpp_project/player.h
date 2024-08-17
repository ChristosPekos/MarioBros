#pragma once

#include "gameobject.h"
#include <sgg/graphics.h>
#include "box.h"

class Player : public Box, public GameObject
{
	// animated player
	std::vector<std::string> m_sprites;

	std::vector<std::string> m_sprites_big_mario;

	graphics::Brush m_brush_player;

	const float m_accel_horizontal = 30.0f;
	const float m_accel_vertical = 350.1f;
	const float m_max_velocity = 5.0f;
	const float m_gravity = 10.0f;
public:
	float m_vx = 0.0f;
	float m_vy = 0.0f;
	float direction = 1.0f;
	bool jump = false;
	int state_of_player = 1;
	
public:
	void update(float dt) override;
	void draw() override;
	void init() override;
	int getState();
	void setState(int state);
	Player(std::string name) : GameObject(name) {}

protected:
	void debugDraw();
	
	// dynamic motion control
	void movePlayer(float dt);
};
