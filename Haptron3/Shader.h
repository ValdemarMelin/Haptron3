#pragma once

#include <map>
#include <string>
#include <GL/glew.h>

namespace GLUtils {

	class Shader {
		std::map<GLuint, GLuint> shaders;
		GLuint program;
	public:
		Shader(std::map<GLuint, std::string>& sources);
		~Shader();
		inline unsigned int id() {
			return program;
		}
	};
}