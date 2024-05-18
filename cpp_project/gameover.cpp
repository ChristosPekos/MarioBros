#include "gameover.h"
#include "util.h"

void GameOver::init()
{

	setActive(false);
	m_gameover_brush.fill_opacity = 1.0f;
	graphics::setFont("assets\\Games.ttf");
	SETCOLOR(m_gameover_brush.fill_color, 0.3f, 0.1f, 0.2f);
}

void GameOver::update(float dt)
{
	if (graphics::getKeyState(graphics::SCANCODE_ESCAPE))
	{
		setActive(false);
	}
}

void GameOver::draw()
{
	float w = m_state->getCanvasWidth()/2;
	float h = m_state->getCanvasHeight()/2;

	if (!getWin()) {
		graphics::drawText(5.0f, h, 1.5f, "GAME OVER", m_gameover_brush);
		graphics::drawText(7.0f, h + 1, 0.5f, "press ESC for exit", m_gameover_brush);
	}
	else {
		graphics::drawText(2.0f, h, 1.0f, "CONGRATULATIONS! YOU WON!", m_gameover_brush);
		graphics::drawText(7.0f, h + 1, 0.5f, "press ESC for exit", m_gameover_brush);
	}

}
