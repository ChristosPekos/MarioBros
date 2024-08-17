#pragma once
#include "gameobject.h"
#include <sgg/graphics.h>
#include "box.h"


// class that represents enemies

class Enemy : public Box, public GameObject
{
	std::vector<std::string> m_sprites_enemy;

	graphics::Brush m_brush_enemy;
	graphics::Brush m_enemy_brush_debug;

	const float m_accel_horizontal = 20.0f;
	const float m_accel_vertical = 300.1f;
	const float m_max_velocity = 3.0f;
	const float m_gravity = 10.0f;
	
	const float m_block_size = 1.0f;
	std::string m_asset_name;


public:
	float m_vx = 0.0f;
	float m_vy = 0.0f;
	bool direction = false;
	int move_flag = 1;
	float move = 1.0f;

	bool alive = true;
	
	

public:
	void update(float dt) override;
	void draw() override;
	void init() override;
	void init(float pos_x, float pos_y);
	bool getCollision();
	void setCollision(bool collide);
	Enemy(float x, float y, float w, float h, const std::string& asset);
	~Enemy();


protected:
	void debugDraw();

	void moveEnemy(float dt);
};