#pragma once
#include "level.h"
#include "sgg/graphics.h"


class GameOver : public Level {

	graphics::Brush m_gameover_brush;

public:
	void init() override;
	void update(float dt) override;
	void draw() override;


	GameOver(std::string name) : Level(name) {}


};