#pragma once

#include "HaptronGame.h"
#include "Graphics.h"

namespace Haptron {

	class Board;
	class Level;
	class BoardGraphics;
	class LSDTile;

	class Board {
		BoardGraphics graphics;
		bool tiles[2][300][300];
		int tn;
	public:
		Board();
		~Board();
		void tick(double dt);
		inline bool tile_at(int x, int y) const { return tiles[tn][x][y]; }
	};

	class Level {
		Board board;
		Level();
	public:
		~Level();
		void tick(double dt);
		inline Board* get_board() { return &board; }
		static Level* create_test_level();
	};

	class BoardGraphics : public Graphics::SceneComponent {
		Board* board;
		static GLUtils::Shader* load_shader();
		static Graphics::GraphicsResource<GLUtils::Shader, BoardGraphics::load_shader> shader;

		GLuint vao, vbo, ibo, tbo;
		void create_vao();
	public:
		BoardGraphics(Board* board);
		~BoardGraphics();
		void render();
	};

	class LSDTile {
		static GLUtils::Shader* load_shader();
		static Graphics::GraphicsResource<GLUtils::Shader, LSDTile::load_shader> shader;
		GLuint vao, vbo, ibo, tbo;
		void create_vao();
	public:
		LSDTile();
		~LSDTile();
		void render();
	};
}