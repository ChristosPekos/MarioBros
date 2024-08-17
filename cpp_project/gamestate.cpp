#include "gamestate.h"
#include "level.h"
#include "player.h"
#include "menu.h"
#include "gameover.h"
#include <thread>
#include <chrono>
#include "enemy.h"
using namespace std::chrono_literals;


// class that represents game state


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


// returns game state instance

GameState* GameState::getInstance()
{
	if (!m_unique_instance)
	{
		m_unique_instance = new GameState();
	}
	return m_unique_instance;
}

// initialization 

bool GameState::init()
{
	graphics::playSound(getFullAssetPath("theme.wav"), 0.5f, true);

	m_menu = new Menu("Menu");
	m_menu->init();

	m_player = new Player("Player");
	m_player->init();

	m_gameover = new GameOver("GameOver");
	m_gameover->init();


	graphics::preloadBitmaps(getAssetDir());

	return true;
}


// draw function


void GameState::draw()
{

	if (!m_menu)
		return;

	if (m_menu->isActive())			// if menu is active, draw it
		m_menu->draw();
	else if (m_current_level) {
		m_current_level->draw();		// else if level is active, draw it
	}
	else if (m_gameover && m_gameover->isActive())		// finally else if game over is active, draw it
		m_gameover->draw();


}


// update function

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

	if (m_menu->isActive()) {		// if menu is active, update it 
		m_menu->update(dt);
	}
	else if (m_current_level) {		// else if level is active, update it
		m_current_level->update(dt);

		if (m_player->m_pos_x >= 191.0f) {			// if player is at the end of the level
			if (num_level == 1) {					// if its the first level
				loadLevel("level2.txt");			// create the second level
			}
			else if (num_level == 2) {				// else if its the second level
				m_gameover->setActive(true);		// set active the game over 
				endofGame();						// delete the level
			}
		}
		else if (m_player->getState() == 0) {		// if the player didn't reach the end but is dead
			m_gameover->setActive(true);			// set active the game over
			endofGame();							// delete the level
		}
	}
	else if (m_gameover && m_gameover->isActive())		 // if game over is active, update it
		m_gameover->update(dt);


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


// function for loading level

void GameState::loadLevel(const std::string& asset)
{
	num_level += 1;
	delete m_current_level;
	m_current_level = nullptr;		//delete previous level
	m_current_level = new Level(asset);
	m_current_level->init();
	m_player->m_pos_x = 3.0f;			//reset player's position
	m_player->m_pos_y = 1.0f;
}


// function for deleting level

void GameState::endofGame()
{
	delete m_current_level;
	m_current_level = nullptr;
	num_level = 0;
}

GameState* GameState::m_unique_instance = nullptr;
