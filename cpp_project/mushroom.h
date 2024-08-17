#pragma once
#include "gameobject.h"
#include "box.h"
#include "sgg/graphics.h"

class Mushroom : public GameObject, public Box {
	graphics::Brush m_block_brush;
	graphics::Brush m_block_brush_debug;
	const float m_block_size = 1.0f;
	std::string m_asset_name;

public:
	void update(float dt) override;
	void draw() override;
	void init() override;
	Mushroom(float x, float y, float w, float h, bool show, const std::string& asset);
	~Mushroom();


};