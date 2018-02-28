
#include "Haptron.h"
#include <gl\glew.h>
#include <map>

using namespace std;

static const char* vs_source = "\
#version 440\r\n\
layout(location = 0) in vec3 vertexPosition_modelspace;\r\n\
uniform mat4 proj;\r\n\
uniform mat4 model;\r\n\
out vec4 pos;\r\n\
void main() {\r\n\
   gl_Position = proj*model*vec4(vertexPosition_modelspace, 1.0);\r\n\
   pos = gl_Position;\r\n\
}\r\n\
";

static const char* fs_source = "\r\n\
#version 440\r\n\
in vec4 pos;\r\n\
uniform vec4 color;\r\n\
void main() {\r\n\
	gl_FragColor = color;\r\n\
}\r\n\
";

namespace Haptron {
	Graphics::GraphicsResource<GLUtils::Shader, BoardGraphics::load_shader> BoardGraphics::shader;

	GLUtils::Shader * BoardGraphics::load_shader()
	{
		map<GLuint, string> sm;
		sm[GL_VERTEX_SHADER] = vs_source;
		sm[GL_FRAGMENT_SHADER] = fs_source;
		return new GLUtils::Shader(sm);
	}

	void BoardGraphics::create_vao()
	{
		GLfloat coordinates[]{
			0.0, 0.0, 0.0,
			1.0, 0.0, 0.0,
			0.0, 1.0, 0.0,
			1.0, 1.0, 0.0
		};
		GLfloat tex_coords[]{ 0, 0, 1, 0, 0, 1, 1, 1 };
		GLuint indexes[]{ 0, 2, 1, 2, 3, 1 };
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		// Generate and bind the vertex buffer object
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(GLfloat), coordinates, GL_STATIC_DRAW);
		// Bind to attribute 0
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// Generate and bind the texture buffer object
		//glGenBuffers(1, &tbo);
		//glBindBuffer(GL_ARRAY_BUFFER, tbo);
		//glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(GLfloat), coordinates, GL_STATIC_DRAW);
		// Bind to attribute 1
		//glEnableVertexAttribArray(1);
		//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

		// Generate and bind the index buffer object
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indexes, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		int v = glGetError();
	}

	BoardGraphics::BoardGraphics(Board* board) : board(board)
	{
		create_vao();
	}

	BoardGraphics::~BoardGraphics()
	{

	}

	void BoardGraphics::render()
	{
		static double t = 0;
		t += 0.1;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glTranslatef(-0.5, -0.5, 0);
		glLoadIdentity();

		glUseProgram(shader->id());
		glBindVertexArray(vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		float model[16] =
		{ 0.003, 0, 0, 0,
			0, 0.003, 0, 0,
			0, 0, 0.003, 0,
			0, 0, 0, 1,
		};
		float color[4] = { 0, 1, 0, 1 };
		GLuint u_proj = glGetUniformLocation(shader->id(), "proj");
		GLuint u_model = glGetUniformLocation(shader->id(), "model");
		GLuint u_color = glGetUniformLocation(shader->id(), "color");
		glUniformMatrix4fv(u_proj, 1, true, Graphics::render_unit.projection_matrix);
		for (int i = 0; i < 300; i++) for (int j = 0; j < 300; j++)
		{
			if (!board->tile_at(i, j)) continue;
			float d1 = sqrtf(powf(i / 300.0 - 0.7, 2) + powf(j / 300.0 - 0.7, 2)) * 40;
			float d2 = sqrtf(powf(i / 300.0 - 0.2, 2) + powf(j / 300.0 - 0.2, 2)) * 40;
			color[0] = (sinf(t + d1) + sinf(t + d2))*0.25 + 0.5;
			color[1] = (sinf(t*1.2 + d1) + sinf(t*1.8 + d2))*0.25 + 0.5;
			color[2] = (sinf(t*1.4 + d1) + sinf(t*1.6 + d2))*0.25 + 0.5;
			model[3] = i * 0.003 - 0.5;
			model[7] = j * 0.003 - 0.5;
			glUniformMatrix4fv(u_model, 1, true, model);
			glUniform4fv(u_color, 1, color);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		int v = glGetError();
	}
	Board::Board():tn(0), graphics(this)
	{
		for (int i = 0; i < 300; i++)
			for (int j = 0; j < 300; j++)
			{
				//int r = rand() % 20;
				//if(r < 3) tiles[tn][i][j] = true;
				//else tiles[tn][i][j] = false;
				tiles[tn][i][j] = true;
			}
	}

	void Board::tick(double dt)
	{
		/*
		int tm = (tn + 1) % 2;
		for (int i = 0; i < 300; i++)
			for (int j = 0; j < 300; j++)
			{
				int r = 0;

				if (tile_at(i - 1, j-1)) r++;
				if (tile_at(i - 1, j)) r++;
				if (tile_at(i - 1, j+1)) r++;

				if (tile_at(i, j - 1)) r++;
				if (tile_at(i, j + 1)) r++;

				if (tile_at(i + 1, j - 1)) r++;
				if (tile_at(i + 1, j)) r++;
				if (tile_at(i + 1, j + 1)) r++;

				if (!tile_at(i, j) && (r == 3) || tile_at(i, j) && (r == 2 || r == 3))
					tiles[tm][i][j] = true;
				else
					tiles[tm][i][j] = false;
			}
		tn = tm;
		*/
	}

	Board::~Board()
	{

	}
}