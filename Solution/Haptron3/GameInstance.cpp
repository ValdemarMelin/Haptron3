
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

	void GameInstance::set_stage(GameStage* level)
	{
		delete this->stage;
		this->stage = level;
		Graphics::render_unit.get_scene().clear();
		level->build_scene();
	}

	void GameInstance::start()
	{
		this->state = GameState::PLAY;
		set_stage(new Level());
	}

	void GameInstance::quit()
	{
		this->state = GameState::QUIT;
		delete this->stage;
		this->stage = nullptr;
	}

	void GameInstance::tick()
	{
		double dt = 0.01;
		stage->tick(dt);
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