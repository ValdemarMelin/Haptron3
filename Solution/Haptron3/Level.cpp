
#include "Haptron.h"

/*
Level definition
*/
namespace Haptron {

	Level::Level()
	{

	}

	Level::~Level()
	{

	}

	void Level::tick(double dt)
	{
		board.tick(dt);
	}

	void Level::build_scene()
	{
		board.scene_init();
	}
}
