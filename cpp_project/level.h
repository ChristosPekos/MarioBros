#pragma once

#include "gameobject.h"
#include <vector>
#include <list>
#include <string>
#include <sgg/graphics.h>
#include "player.h"
#include "staticblock.h"
#include "dynamicblock.h"
#include "mushroom.h"
#include "Enemy.h"
#include "gamestate.h"

class Level : public GameObject
{
	graphics::Brush m_brush_background;

	std::vector<StaticBlocks*> m_static_objects;
	std::list<DynamicBlocks*> m_dynamic_objects;
	std::list<Mushroom*> m_mushrooms;
	std::list<Enemy*> m_enemies;

	std::string text_path;
	float row = 0.0f;
	float col = 0.0f;



	// detect collisions
	void checkCollisions();

	void checkCollisions(Enemy& enemy);

public:
	void update(float dt) override;
	void draw() override;
	void init() override;
	void readTxt(std::string name);
	void resetLevel();
	Level(const std::string& name) : GameObject(name) {}
	~Level() override;
};
