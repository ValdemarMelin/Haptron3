
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

	Level* Level::create_test_level()
	{
		return new Level();
	}

	void Level::tick(double dt)
	{
		board.tick(dt);
	}
}
