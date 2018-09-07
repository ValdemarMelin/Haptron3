#pragma once

#include "Graphics.h"
#include "HaptronGame.h"

namespace Haptron {

	class Board;
	class Level;
	class BoardGraphics;

	class BoardGraphics : public Graphics::SceneComponent {
		static GLUtils::Shader* load_shader();
		static Graphics::GraphicsResource<GLUtils::Shader, BoardGraphics::load_shader> shader;

		Board* board;
		GLuint vao, vbo, ibo, tbo;
		void create_vao();
	public:
		BoardGraphics(Board* board);
		~BoardGraphics();
		void render();
		void init();
		void dispose();
	};

	class Board {
		BoardGraphics graphics;
	public:
		Board();
		~Board();
		void tick(double dt);
		void scene_init();
	};

	class Level : public GameStage {
		Board board;
	public:
		Level();
		~Level();
		void tick(double dt);
		void build_scene();
		inline Board* get_board() { return &board; }
	};
}