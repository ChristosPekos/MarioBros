#pragma once
#include "level.h"
#include "sgg/graphics.h"


class Menu : public Level {

	graphics::Brush m_menu_brush;


public:
	void init() override;
	void update(float dt) override;
	void draw() override;
	Menu(std::string name) : Level(name) {}
};