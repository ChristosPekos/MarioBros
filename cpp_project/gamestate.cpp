#include "gamestate.h"
#include "level.h"
#include "player.h"
#include "menu.h"
#include "gameover.h"
#include <thread>
#include <chrono>
using namespace std::chrono_literals;

GameState::GameState()
{
}

GameState::~GameState()
{
	if (m_menu)
		delete m_menu;

	if (m_gameover)
		delete m_gameover;

	if (m_current_level)
		delete m_current_level;

	if (m_player)
		delete m_player;
}

GameState* GameState::getInstance()
{
	if (!m_unique_instance)
	{
		m_unique_instance = new GameState();
	}
	return m_unique_instance;
}

bool GameState::init()
{
	/*m_current_level = new Level("1.lvl");
	m_current_level->init();*/

	m_menu = new Menu("Menu");
	m_menu->init();
	
	m_gameover = new GameOver("GameOver");
	m_gameover->init();

	m_player = new Player("Player");
	m_player->init();

	graphics::preloadBitmaps(getAssetDir());

	return true;
}

void GameState::draw()
{

	if (!m_menu)
		return;

	 if (m_menu->isActive())
		m_menu->draw();
	else if (m_gameover && m_gameover->isActive())
		m_gameover->draw();
	else if (m_current_level)
		m_current_level->draw();

	if (m_gameover->isActive())
		m_gameover->draw();

	/*m_current_level->draw();*/
	
	
}

void GameState::update(float dt)
{
	// Skip an update if a long delay is detected 
	// to avoid messing up the collision simulation
	if (dt > 500) // ms
		return;
	
	// Avoid too quick updates
	float sleep_time = std::max(17.0f - dt, 0.0f);
	if (sleep_time > 0.0f)
	{
		std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(sleep_time));
	}

	if (!m_menu)
		return;

	if (m_menu->isActive())
		m_menu->update(dt);
	else if (m_gameover && m_gameover->isActive())
		m_gameover->update(dt);
	else if (m_current_level)
		m_current_level->update(dt);
	m_debugging = graphics::getKeyState(graphics::SCANCODE_0);

}

std::string GameState::getFullAssetPath(const std::string& asset)
{
	return m_asset_path + asset;
}

std::string GameState::getAssetDir()
{
	return m_asset_path;
}

void GameState::loadLevel(const std::string& asset)
{
	delete m_current_level;
	m_current_level = nullptr;
	m_current_level = new Level(asset);
	m_current_level->init();

}

GameState* GameState::m_unique_instance = nullptr;