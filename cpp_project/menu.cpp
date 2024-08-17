#include "menu.h"

// class that reprsents the main menu

void Menu::init()
{
	setActive(true);
	m_menu_brush.fill_opacity = 1.0f;
	m_menu_brush.texture = m_state->getFullAssetPath("menu.png");
}

void Menu::update(float dt)
{
	if (graphics::getKeyState(graphics::SCANCODE_SPACE)) 
	{
		m_state->loadLevel("level1.txt");			//press space to start
		setActive(false);				// inactivate it
	}
}

void Menu::draw()
{
	float w = m_state->getCanvasWidth();
	float h = m_state->getCanvasHeight();
	graphics::drawRect(w/2, h/2, w, h, m_menu_brush);
}
