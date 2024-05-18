#include "level.h"
#include <sgg/graphics.h>
#include "player.h"
#include "util.h"
#include <iostream>
#include <fstream>
#include "staticblock.h"
#include "dynamicblock.h"



void Level::checkCollisions()
{

	for (auto& block : m_static_objects)
	{
		float offset = 0.0f;
		if (offset = m_state->getPlayer()->intersectDown(*block))
		{
			m_state->getPlayer()->m_pos_y += offset ;

			m_state->getPlayer()->m_vy = 0.0f;

			break;
		}
	}
	for (auto& block : m_static_objects)
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

	


	for (auto& block : m_dynamic_objects)
	{
		float offset = 0.0f;
		if (offset = m_state->getPlayer()->intersectDown(*block))
		{
			m_state->getPlayer()->m_pos_y += offset;

			// add sound event
			if (m_state->getPlayer()->m_vy > 1.0f)
				graphics::playSound(m_state->getFullAssetPath("Metal2.wav"), 1.0f);

			m_state->getPlayer()->m_vy = 0.0f;

			break;
		}
	}
	
	for (auto& block : m_dynamic_objects)
	{
		float offset = 0.0f;
		if (offset = block->intersectDown(*m_state->getPlayer()))
		{
			m_state->getPlayer()->m_pos_y -= offset;
			
			// add sound event
			if (m_state->getPlayer()->m_vy > 1.0f)
				graphics::playSound(m_state->getFullAssetPath("Metal2.wav"), 1.0f);

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

	}


}


	


void Level::update(float dt)
{
	if (m_state->getPlayer()->isActive())
		m_state->getPlayer()->update(dt);


	checkCollisions();

	GameObject::update(dt);
}

void Level::draw()
{
	float w = m_state->getCanvasWidth();
	float h = m_state->getCanvasHeight();
	
	float offset_x = m_state->m_global_offset_x / 2.0f + w/2.0f;

	//draw background
	graphics::drawRect( offset_x, h / 2.0f, 12.0f*w, 2.0f*h, m_brush_background);

	// draw player
	if (m_state->getPlayer()->isActive())
		m_state->getPlayer()->draw();


	for (auto& block : m_static_objects)
		block->draw();

	for (auto& block : m_dynamic_objects)
		block->draw();

}

void Level::init()
{
	m_brush_background.outline_opacity = 0.0f;
	m_brush_background.texture = m_state->getFullAssetPath("BackgroundImage.png");

	// Stage 1
	readTxt(m_name);

	for (auto p_gob : m_static_objects)
		if (p_gob) p_gob->init();
	
	for (auto p_gob : m_dynamic_objects)
		if (p_gob) p_gob->init();

}

void Level:: readTxt(std::string name)
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
}
