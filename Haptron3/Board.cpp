
#include "Haptron.h"
#include <gl\glew.h>
#include <map>

using namespace std;

namespace Haptron {
	Graphics::GraphicsResource<GLUtils::Shader, BoardGraphics::load_shader> BoardGraphics::shader;

	GLUtils::Shader * BoardGraphics::load_shader()
	{
		return nullptr;
	}

	void BoardGraphics::create_vao()
	{

	}

	BoardGraphics::BoardGraphics(Board* board) : board(board)
	{

	}

	BoardGraphics::~BoardGraphics()
	{

	}

	void BoardGraphics::init()
	{
		create_vao();
	}

	void BoardGraphics::dispose()
	{
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ibo);
		glDeleteVertexArrays(1, &vao);
	}

	void BoardGraphics::render()
	{

	}

	Board::Board(): graphics(this)
	{

	}

	Board::~Board()
	{
		Graphics::render_unit.get_scene().remove(&this->graphics);
	}

	void Board::scene_init()
	{
		Graphics::render_unit.get_scene().add(&this->graphics);
	}


	void Board::tick(double dt)
	{

	}
}