#include "player.h"
#include "util.h"
#include <cmath>
#include "gameover.h"



// class that represents the player

void Player::update(float dt)
{
	float delta_time = dt / 1000.0f;



	if (m_pos_y > 15.0f)
		state_of_player = 0;


	movePlayer(dt);

	// update offset for other game objects
	m_state->m_global_offset_x = m_state->getCanvasWidth() / 2.0f - m_pos_x;

	GameObject::update(dt);

}



void Player::draw()
{
	if (state_of_player == 1) {			// small mario state
		int sprite = (int)fmod(1000.0f - m_pos_x * 3.0f, m_sprites.size());			// create an index to draw differnt images so it looks like walking

		m_brush_player.texture = m_sprites[sprite];
		graphics::setScale(direction, 1.0f);			// if direction is 1.0 draw him standing to the right, else to the left

		if (jump)		// jump animation
			m_brush_player.texture = m_state->getFullAssetPath("mariojumping.png");


		graphics::drawRect(m_state->getCanvasWidth() * 0.5f, m_pos_y, 1.0f, 1.0f, m_brush_player);
		graphics::resetPose();


		if (m_state->m_debugging)
			debugDraw();
	}
	else if (state_of_player == 2) {		// big mario state  (same as above)
		int sprite = (int)fmod(1000.0f - m_pos_x * 3.0f, m_sprites_big_mario.size());

		m_brush_player.texture = m_sprites_big_mario[sprite];
		graphics::setScale(direction, 1.0f);

		if (jump)
			m_brush_player.texture = m_state->getFullAssetPath("bigmariojumping.png");


		graphics::drawRect(m_state->getCanvasWidth() * 0.5f, m_pos_y, 1.0f, 1.0f, m_brush_player);
		graphics::resetPose();


		if (m_state->m_debugging)
			debugDraw();

	}
	else if (state_of_player <= 0) {			// dead mario state
		m_brush_player.texture = m_state->getFullAssetPath("mariodeath.png");


		graphics::drawRect(m_state->getCanvasWidth() * 0.5f, m_pos_y, 1.0f, 1.0f, m_brush_player);

		if (m_state->m_debugging)
			debugDraw();

	}
}

void Player::init()
{
	// stage 1
	m_pos_x = 3.0f;
	m_pos_y = 1.0f;

	direction = 1.0f;
	jump = false;


	m_state->m_global_offset_x = m_state->getCanvasWidth() / 2.0f - 6.0f; // create an offset so the other objects follow mario

	m_brush_player.fill_opacity = 1.0f;
	m_brush_player.outline_opacity = 0.0f;
	m_brush_player.texture = m_state->getFullAssetPath("mariostanding1.png");

	m_sprites.push_back(m_state->getFullAssetPath("marioright0.png"));
	m_sprites.push_back(m_state->getFullAssetPath("marioright1.png"));
	m_sprites.push_back(m_state->getFullAssetPath("marioright2.png"));

	m_sprites_big_mario.push_back(m_state->getFullAssetPath("bigmariostanding1.png"));
	m_sprites_big_mario.push_back(m_state->getFullAssetPath("bigmarioright0.png"));
	m_sprites_big_mario.push_back(m_state->getFullAssetPath("bigmarioright1.png"));
	m_sprites_big_mario.push_back(m_state->getFullAssetPath("bigmarioright2.png"));

	// Adjust width for finer collision detection
	m_width = 0.85f;
}

void Player::debugDraw()
{
	graphics::Brush debug_brush;
	SETCOLOR(debug_brush.fill_color, 1, 0.3f, 0);
	SETCOLOR(debug_brush.outline_color, 1, 0.1f, 0);
	debug_brush.fill_opacity = 0.1f;
	debug_brush.outline_opacity = 1.0f;
	graphics::drawRect(m_state->getCanvasWidth() * 0.5f, m_pos_y, m_width, m_height, debug_brush);

	char s[20];
	sprintf_s(s, "(%5.2f, %5.2f)", m_pos_x, m_pos_y);
	SETCOLOR(debug_brush.fill_color, 1, 0, 0);
	debug_brush.fill_opacity = 1.0f;
	graphics::drawText(m_state->getCanvasWidth() * 0.5f - 0.4f, m_pos_y - 0.6f, 0.15f, s, debug_brush);
}


// function that implements the movements


void Player::movePlayer(float dt)
{
	if (m_vy == 0.0f)
		jump = false;


	float delta_time = dt / 1000.0f;


	if (state_of_player > 0) {		// if  not dead
		// Stage 2 code: Acceleration-based velocity
		float move = 0.0f;
		if (graphics::getKeyState(graphics::SCANCODE_A)) {			// mov left
			move -= 1.0f;
			direction = -1.0f;
		}
		if (graphics::getKeyState(graphics::SCANCODE_D)) {			// move right
			move = 1.0f;
			direction = 1.0f;
		}

		m_vx = std::min<float>(m_max_velocity, m_vx + delta_time * move * m_accel_horizontal);		// increase velocity
		m_vx = std::max<float>(-m_max_velocity, m_vx);

		// friction
		m_vx -= 0.2f * m_vx / (0.1f + fabs(m_vx));

		// apply static friction threshold
		if (fabs(m_vx) < 0.01f)
			m_vx = 0.0f;

		// adjust horizontal position
		m_pos_x += m_vx * delta_time;

		// jump only when not in flight:
		if (m_vy == 0.0f)
			if (graphics::getKeyState(graphics::SCANCODE_W)) {
				m_vy -= m_accel_vertical * 0.02f;// not delta_time!! Burst 
				if (state_of_player == 1)		// jumping sounds
					graphics::playSound(m_state->getFullAssetPath("jump-small.wav"), 0.8f);
				else if (state_of_player == 2)
					graphics::playSound(m_state->getFullAssetPath("jump-super.wav"), 0.8f);
				jump = true;
			}
	}

	// add gravity
	m_vy += delta_time * m_gravity;

	// adjust vertical position
	m_pos_y += m_vy * delta_time;



}

// setter and getter of player's state

void Player::setState(int state) {		
	state_of_player = state;
}

int Player::getState() {
	return state_of_player;
}
