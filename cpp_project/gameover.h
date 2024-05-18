#pragma once
#include "level.h"
#include "sgg/graphics.h"


class GameOver : public Level {

	graphics::Brush m_gameover_brush;
	bool win;

public:
	void init() override;
	void update(float dt) override;
	void draw() override;
	void setWin(bool a) { win = a; }
	bool getWin() { return win; }

	GameOver(std::string name) : Level(name), win(true) {}


};