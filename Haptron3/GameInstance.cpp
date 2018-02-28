
#include "Haptron.h"

/*
GameInstance definition
*/
namespace Haptron {

	GameInstance game;

	GameInstance::GameInstance()
	{

	}

	GameInstance::~GameInstance()
	{

	}

	void GameInstance::set_level(Level* level)
	{
		delete this->level;
		this->level = level;
		Graphics::render_unit.set_level_graphics(level);
	}

	void GameInstance::start()
	{
		this->state = GameState::PLAY;
		set_level(Level::create_test_level());
	}

	void GameInstance::quit()
	{
		this->state = GameState::QUIT;
		Graphics::render_unit.set_level_graphics(nullptr);
		delete this->level;
		this->level = nullptr;
	}

	void GameInstance::tick()
	{
		double dt = 0.01;
		level->tick(dt);
	}
	void GameInstance::key_down(int key)
	{
		switch (key)
		{
		case 0x25://left
			pos[0] -= 0.001;
			break;
		case 0x26://up
			pos[1] -= 0.001;
		case 0x27://right
			pos[0] += 0.001;
			break;
		case 0x28://down
			pos[1] += 0.001;
			break;
		}
	}
	void GameInstance::key_up(int key)
	{

	}
}