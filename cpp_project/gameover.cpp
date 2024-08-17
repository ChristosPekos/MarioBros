#include "gameover.h"
#include "util.h"

// ending level (game over) prints if we won or lost


// initialization

void GameOver::init()
{
	setActive(false);
	m_gameover_brush.fill_opacity = 1.0f;
	graphics::setFont("assets\\Games.ttf");
	SETCOLOR(m_gameover_brush.fill_color, 0.3f, 0.1f, 0.2f);
}

// update

void GameOver::update(float dt)
{
	if (graphics::getKeyState(graphics::SCANCODE_ESCAPE))    // press esc to quit
	{
		setActive(false);
	}
}

void GameOver::draw()
{
	float w = m_state->getCanvasWidth() / 2;
	float h = m_state->getCanvasHeight() / 2;

	if (m_state->getPlayer()->getState() == 0) {		// if player is dead
		graphics::drawText(5.0f, h, 1.5f, "GAME OVER", m_gameover_brush);
		graphics::drawText(7.0f, h + 1, 0.5f, "press ESC for exit", m_gameover_brush);
	}
	else if (m_state->getPlayer()->m_pos_x >= 191.0f) {			// if player is at the end of the level
		graphics::drawText(2.0f, h, 1.0f, "CONGRATULATIONS! YOU WON!", m_gameover_brush);
		graphics::drawText(7.0f, h + 1, 0.5f, "press ESC for exit", m_gameover_brush);
	}
}
