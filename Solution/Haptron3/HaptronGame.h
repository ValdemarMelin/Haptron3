#pragma once

#include <set>

namespace Haptron {
	// COMPONENTS

	enum GameState {
		PLAY, QUIT
	};

	class GameInstance {
		GameState state;
		Level* level;

		void set_level(Level* level);
	public:
		float pos[3];
		GameInstance();
		~GameInstance();
		inline GameState get_state() const { return state; }
		inline Level* get_level() const { return level; }
		void start();
		void quit();
		void tick();
		void key_down(int key);
		void key_up(int key);
	};

	extern GameInstance game;
}