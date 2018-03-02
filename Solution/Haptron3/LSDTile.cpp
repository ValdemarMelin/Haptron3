#include "Haptron.h"

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
uniform float time;\r\n\
uniform vec2 p1;\r\n\
uniform vec2 p2;\r\n\
void main() {\r\n\
	float r1 = 60*sqrt((pos.x-p1.x)*(pos.x-p1.x) + (pos.y-p1.y)*(pos.y-p1.y));\r\n\
	float r2 = 60*sqrt((pos.x-p2.x)*(pos.x-p2.x) + (pos.y-p2.y)*(pos.y-p2.y));\r\n\
	gl_FragColor = vec4((sin(r1+time)+sin(r2+time))*0.25 + 0.5, (sin(r1 + 1.1f*time)+sin(r2 + 1.1f*time))*0.25 + 0.5, (sin(r1 + 1.2f*time)+sin(r2 + 1.2f*time))*0.25 + 0.5, 1); \r\n\
}\r\n\
";

namespace Haptron {
	namespace Graphics {
		GraphicsResource<GLUtils::Shader, LSDTile::load_shader> LSDTile::shader;

		GLUtils::Shader * LSDTile::load_shader()
		{
			map<GLuint, string> sm;
			sm[GL_VERTEX_SHADER] = vs_source;
			sm[GL_FRAGMENT_SHADER] = fs_source;
			return new GLUtils::Shader(sm);
		}

		void LSDTile::create_vao()
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

			glGenBuffers(1, &ibo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indexes, GL_STATIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
			int v = glGetError();
		}

		LSDTile::LSDTile()
		{
			create_vao();
		}

		LSDTile::~LSDTile()
		{

		}

		void LSDTile::render()
		{

			static double t = 0;
			t += 0.01;
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glTranslatef(-0.5, -0.5, 0);
			glLoadIdentity();

			glUseProgram(shader->id());
			glBindVertexArray(vao);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
			float model[16] =
			{ 1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1,
			};
			GLuint u_proj = glGetUniformLocation(shader->id(), "proj");
			GLuint u_model = glGetUniformLocation(shader->id(), "model");
			GLuint u_p1 = glGetUniformLocation(shader->id(), "p1");
			GLuint u_p2 = glGetUniformLocation(shader->id(), "p2");
			GLuint u_time = glGetUniformLocation(shader->id(), "time");

			float p1[2]{game.pos[0], game.pos[1]};
			float p2[2]{-game.pos[0],-game.pos[1]};
			model[3] = -0.5;
			model[7] = -0.5;

			glUniformMatrix4fv(u_proj, 1, true, render_unit.projection_matrix);
			glUniform2fv(u_p1, 1, p1);
			glUniform2fv(u_p2, 1, p2);
			glUniform1f(u_time, t);
			glUniformMatrix4fv(u_model, 1, true, model);
			int v = glGetError();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
	}
}