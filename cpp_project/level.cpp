#include "level.h"
#include <sgg/graphics.h>
#include "player.h"
#include "util.h"
#include <iostream>
#include <fstream>
#include "staticblock.h"
#include "dynamicblock.h"
#include "mushroom.h"
#include "Enemy.h"

// class that represents the level

void Level::checkCollisions()
{

	for (auto& block : m_static_objects)			// for each static block
	{
		float offset = 0.0f;
		if (offset = m_state->getPlayer()->intersectDown(*block))			// check if player intersects down with a block, if true move the player's x position by offset
		{
			m_state->getPlayer()->m_pos_y += offset;

			m_state->getPlayer()->m_vy = 0.0f;

			break;
		}
	}
	for (auto& block : m_static_objects)			// for each static block
	{
		float offset = 0.0f;
		if (offset = m_state->getPlayer()->intersectSideways(*block)) // check if player intersects sideways with a block, if true move the player's x position by offset
		{
			m_state->getPlayer()->m_pos_x += offset;

			m_state->getPlayer()->m_vx = 0.0f;
			break;
		}
		else if (offset = block->intersectDown(*m_state->getPlayer()))		// if it doesn't intersect sideways, then check if player intersects up with a block, if true move the player's y position by offset
		{
			m_state->getPlayer()->m_pos_y -= offset;
			m_state->getPlayer()->m_vy = 0.1f;
			graphics::playSound(m_state->getFullAssetPath("bump.wav"), 0.8f);


			break;

		}
	}





	for (auto& block : m_dynamic_objects)		// same for dynamics
	{
		float offset = 0.0f;
		if (offset = m_state->getPlayer()->intersectDown(*block))
		{
			m_state->getPlayer()->m_pos_y += offset;

			// add sound event
			if (m_state->getPlayer()->m_vy > 1.0f)

			m_state->getPlayer()->m_vy = 0.0f;

			break;
		}
	}

	for (auto& block : m_dynamic_objects)
	{
		float offset = 0.0f;
		if (offset = m_state->getPlayer()->intersectSideways(*block))
		{
			m_state->getPlayer()->m_pos_x += offset;

			m_state->getPlayer()->m_vx = 0.0f;
			break;
		}
		else if (offset = block->intersectDown(*m_state->getPlayer()))
		{
			m_state->getPlayer()->m_pos_y -= offset;
			m_state->getPlayer()->m_vy = 0.1f;
			graphics::playSound(m_state->getFullAssetPath("bump.wav"), 0.8f);


			break;

		}
	}

	for (auto& block : m_enemies)			// check if player intersect with enemies
	{
		if (block->isActive()) {
			float offset = 0.0f;
			if (offset = m_state->getPlayer()->intersectSideways(*block))		// if they intersect sideways, remove a life from player and move him horizontallly by offse
			{
				m_state->getPlayer()->m_pos_x += offset;
				m_state->getPlayer()->m_pos_y = 3.0f;

				m_state->getPlayer()->m_vx = 0.0f;

				if (block->isActive())
					m_state->getPlayer()->state_of_player -= 1;		// remove a life

				if (m_state->getPlayer()->getState() == 0)
					graphics::playSound(m_state->getFullAssetPath("mariodie.wav"), 0.8f);

				break;
			}
			else if (offset = m_state->getPlayer()->intersectDown(*block))				// else if we have an down intersection kill enemy
			{
				m_state->getPlayer()->m_pos_y += offset;

				graphics::playSound(m_state->getFullAssetPath("bump.wav"), 0.8f);

				m_state->getPlayer()->m_vy = 0.0f;

				block->setActive(false);

				break;
			}
		}
	}


	for (auto& block : m_mushrooms)			// check sideways intersection with mushrooms, if its true the "kill" the mushroom
	{
		float offset = 0.0f;
		if (offset = m_state->getPlayer()->intersectSideways(*block))
		{
			graphics::playSound(m_state->getFullAssetPath("powerup.wav"), 0.8f);
			m_state->getPlayer()->m_pos_x += offset;

			m_state->getPlayer()->m_vx = 0.0f;



			m_state->getPlayer()->state_of_player = 2;			// make small mario, big mario (power up)

			m_mushrooms.remove(block);

			break;
		}
	}






}


// function that handles the collisions between enemies and blocks (same as with the player's)

void Level::checkCollisions(Enemy& enemy) {



	for (auto& block : m_static_objects)
	{
		float offset = 0.0f;
		if (offset = enemy.intersectDown(*block))
		{
			//enemy.m_pos_y += offset;

			enemy.m_vy = 0.0f;

			break;
		}
	}



	for (auto& block : m_static_objects)
	{
		float offset = 0.0f;

		if (offset = enemy.intersectSideways(*block))
		{

			enemy.m_vx = 0.0f;

			if (enemy.move == -1.0f) {
				enemy.move = 1.0f;
				enemy.m_pos_x += offset;

				enemy.m_vx = 0.0f;
			}
			else {
				enemy.move = -1.0f;
			}
			break;
		}


	}





}


// update function

void Level::update(float dt)
{

	if (m_state->getPlayer()->isActive())
		m_state->getPlayer()->update(dt);

	for (auto& block : m_enemies) {
		if (block->isActive()) {

			block->update(dt);
		}
	}

	checkCollisions();

	for (auto& block : m_enemies) {
		checkCollisions(*block);
	}


	GameObject::update(dt);
}


// draw function


void Level::draw()
{
	float w = m_state->getCanvasWidth();
	float h = m_state->getCanvasHeight();

	float offset_x = m_state->m_global_offset_x / 2.0f + w / 2.0f;		// draw background so it gives the illusion that the enemy is moving

	//draw background
	graphics::drawRect(offset_x, h / 2.0f, 12.0f * w, 2.0f * h, m_brush_background);


	for (auto& block : m_enemies)
		block->draw();

	// draw player
	if (m_state->getPlayer()->isActive())
		m_state->getPlayer()->draw();


	for (auto& block : m_static_objects)
		block->draw();

	for (auto& block : m_dynamic_objects)
		block->draw();

	for (auto& block : m_mushrooms)

		block->draw();

}

void Level::init()
{

	m_brush_background.outline_opacity = 0.0f;
	m_brush_background.texture = m_state->getFullAssetPath("BackgroundImage.png");

	readTxt(m_name);

	for (auto p_gob : m_static_objects)
		if (p_gob) p_gob->init();

	for (auto p_gob : m_dynamic_objects)
		if (p_gob) p_gob->init();

	for (auto p_gob : m_mushrooms)
		if (p_gob) p_gob->init();

	for (auto p_gob : m_enemies)
		if (p_gob) p_gob->init();

}



// function for reading the txt that contains the level

void Level::readTxt(std::string name)
{
	text_path = m_state->getFullAssetPath(name);
	std::ifstream file(text_path);

	if (file.is_open()) {
		std::string line;
		col = 1.0f;
		while (std::getline(file, line))
		{
			row = -6.0f;
			for (char object : line) {
				if (object == '.') {
					row++;
					continue;
				}
				else if (object == '0') {
					m_static_objects.push_back(new StaticBlocks(row, col, 1.0f, 1.0f, "ground.png"));
				}
				else if (object == '1') {
					m_static_objects.push_back(new StaticBlocks(row, col, 1.0f, 1.0f, "ground1.png"));
				}
				else if (object == '2') {
					m_static_objects.push_back(new StaticBlocks(row, col, 1.0f, 1.0f, "step.png"));
				}
				else if (object == '?') {
					m_dynamic_objects.push_back(new DynamicBlocks(row, col, 1.0f, 1.0f, true, "questionmarktile.png"));
				}
				else if (object == 'b') {
					m_static_objects.push_back(new StaticBlocks(row, col, 1.0f, 1.0f, "pipebody.png"));
				}
				else if (object == 'h') {
					m_static_objects.push_back(new StaticBlocks(row, col, 1.0f, 1.0f, "pipehead.png"));
				}
				else if (object == 'g') {
					m_static_objects.push_back(new StaticBlocks(row, col, 1.0f, 1.0f, "finishbody.png"));
				}
				else if (object == 'f') {
					m_static_objects.push_back(new StaticBlocks(row, col, 1.0f, 1.0f, "finishhead.png"));
				}
				else if (object == 'e') {
					m_enemies.push_back(new Enemy(row, col, 1.0f, 1.0f, "enemy0.png"));
				}
				else if (object == 'm') {
					m_mushrooms.push_back(new Mushroom(row, col, 1.0f, 1.0f, true, "mushroom.png"));
				}
				row++;
			}
			col++;
		}
		file.close();
	}
	else {
		std::cerr << "Couldn't open file." << std::endl;
	}

}


Level::~Level()
{
	for (auto p_go : m_static_objects)
		delete p_go;
	for (auto p_go : m_dynamic_objects)
		delete p_go;
	for (auto p_go : m_mushrooms)
		delete p_go;
	for (auto p_go : m_enemies)
		delete p_go;
}
