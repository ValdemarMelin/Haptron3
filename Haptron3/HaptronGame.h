#pragma once

#include <set>

namespace Haptron {
	// COMPONENTS

	class GameStage {

	public:
		inline GameStage() { }
		virtual ~GameStage() { }
		virtual void tick(double dt) = 0;
		virtual void build_scene() = 0;
	};

	enum GameState {
		PLAY, QUIT
	};

	class GameInstance {
		GameState state;
		GameStage* stage;

	public:
		float pos[3];
		GameInstance();
		~GameInstance();
		inline GameState get_state() const { return state; }
		inline GameStage* get_stage() const { return stage; }
		void set_stage(GameStage* level);
		void start();
		void quit();
		void tick();
		void key_down(int key);
		void key_up(int key);
	};

	extern GameInstance game;
}