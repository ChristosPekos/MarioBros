#include "mushroom.h"
#include "util.h"

// class that reprsents the mushroom


void Mushroom::update(float dt)
{
}

void Mushroom::draw()
{
	if (isActive()) {			// if its active draw it
		float x = m_pos_x + m_state->m_global_offset_x;
		graphics::drawRect(x, m_pos_y, 1.0f * m_block_size, 1.0f * m_block_size, m_block_brush);

		if (m_state->m_debugging)
			graphics::drawRect(x, m_pos_y, m_block_size, m_block_size, m_block_brush_debug);
	}

}

void Mushroom::init()
{
	m_block_brush.outline_opacity = 0.0f;
	m_block_brush_debug.fill_opacity = 0.1f;
	SETCOLOR(m_block_brush_debug.fill_color, 0.1f, 1.0f, 0.1f);
	SETCOLOR(m_block_brush_debug.outline_color, 0.3f, 1.0f, 0.2f);
	m_block_brush.texture = m_state->getFullAssetPath(m_asset_name);
}

Mushroom::Mushroom(float x, float y, float w, float h, bool show, const std::string& asset)
{
	m_pos_x = x;
	m_pos_y = y;
	m_width = w;
	m_height = h;
	m_asset_name = asset;
	m_active = show;
}

Mushroom::~Mushroom()
{
}