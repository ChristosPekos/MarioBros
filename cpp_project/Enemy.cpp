#include "Enemy.h"
#include "util.h"
#include <cmath>


// update function

void Enemy::update(float dt)
{
	
	float delta_time = dt / 500.0f;

	moveEnemy(dt);

	GameObject::update(dt);
	

}


// draw function

void Enemy::draw()
{
	float x = m_pos_x + m_state->m_global_offset_x;
	int sprite = (int)fmod(1000.0f - m_pos_x * 3.0f, m_sprites_enemy.size());			// index to change images so it looks like walking
	m_brush_enemy.texture = m_sprites_enemy[sprite];
	graphics::setScale(direction, 1.0f);


	if (isActive()) {				//if its actve draw it alive 
		graphics::drawRect(x, m_pos_y, 1.0f, 1.0f, m_brush_enemy);
		graphics::resetPose();
		
		if (m_state->m_debugging)
			debugDraw();
	}
	else {				// if its not active draw it dead
		m_brush_enemy.texture = m_state->getFullAssetPath("enemydead.png");
		graphics::drawRect(x, m_pos_y, 1.0f, 1.0f, m_brush_enemy);
		
		if (m_state->m_debugging)
			debugDraw();
	}



}


// initialization

void Enemy::init()
{
	
	direction = 1.0f;

	m_brush_enemy.fill_opacity = 1.0f;
	m_brush_enemy.outline_opacity = 0.0f;
	m_brush_enemy.texture = m_state->getFullAssetPath("enemy0.png");
	m_sprites_enemy.push_back(m_state->getFullAssetPath("enemy0.png"));
	m_sprites_enemy.push_back(m_state->getFullAssetPath("enemy1.png"));
	
}

// second initialization function that starts the enemy from a specific position

void Enemy::init(float pos_x, float pos_y)
{

	m_pos_x = pos_x;
	m_pos_y = pos_y;
	direction = 1.0f;

	m_brush_enemy.fill_opacity = 1.0f;
	m_brush_enemy.outline_opacity = 0.0f;
	m_brush_enemy.texture = m_state->getFullAssetPath("enemy0.png");

	m_sprites_enemy.push_back(m_state->getFullAssetPath("enemy0.png"));
	m_sprites_enemy.push_back(m_state->getFullAssetPath("enemy1.png"));

}


// constructor

Enemy::Enemy(float x, float y, float w, float h, const std::string& asset)
{
	m_pos_x = x;
	m_pos_y = y;
	m_width = w;
	m_height = h;
	m_asset_name = asset;
}

// destructor

Enemy::~Enemy()
{

}


// returns true if it collides with another box

bool Enemy::getCollision()
{
	return direction;
}



// sets true if it collides with another box

void Enemy::setCollision(bool collide)
{
	direction = collide;
}



// debug draw function

void Enemy::debugDraw()
{
	float x = m_pos_x + m_state->m_global_offset_x;
	graphics::Brush debug_brush;
	SETCOLOR(debug_brush.fill_color, 1, 0.3f, 0);
	SETCOLOR(debug_brush.outline_color, 1, 0.1f, 0);
	debug_brush.fill_opacity = 0.1f;
	debug_brush.outline_opacity = 1.0f;
	graphics::drawRect(x, m_pos_y, m_width, m_height, debug_brush);

	char s[20];
	sprintf_s(s, "(%5.2f, %5.2f)", m_pos_x, m_pos_y);
	SETCOLOR(debug_brush.fill_color, 1, 0, 0);
	debug_brush.fill_opacity = 1.0f;
	graphics::drawText(x - 0.4f, m_pos_y - 0.6f, 0.15f, s, debug_brush);
}


// function for enemy movement (only horizontal)

void Enemy::moveEnemy(float dt)
{

	float delta_time = dt / 1000.0f;

	m_vx = std::min<float>(m_max_velocity, m_vx + delta_time * move * m_accel_horizontal);
	m_vx = std::max<float>(-m_max_velocity, m_vx);

	// friction
	m_vx -= 0.2f * m_vx / (0.1f + fabs(m_vx));

	// apply static friction threshold
	if (fabs(m_vx) < 0.01f)
		m_vx = 0.0f;

	// adjust horizontal position
	m_pos_x += m_vx * delta_time;

	// add gravity
	m_vy += delta_time * m_gravity;

	

}