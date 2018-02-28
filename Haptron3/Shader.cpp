
#include "Shader.h"

using namespace std;

GLUtils::Shader::Shader(map<GLuint, string>& shaders) {
	for (auto& p : shaders) {
		GLuint s = glCreateShader(p.first);
		const GLchar* txt = &p.second[0];
		const GLint len = p.second.length();
		glShaderSource(s, 1, &txt, &len);
		glCompileShader(s);
		GLint compiled;
		glGetShaderiv(s, GL_COMPILE_STATUS, &compiled);
		if (!compiled) {
			GLint l;
			glGetShaderiv(s, GL_INFO_LOG_LENGTH, &l);
			string log;
			log.resize(l);
			glGetShaderInfoLog(s, l, NULL, &log[0]);
			throw std::exception(log.c_str());
		}
		this->shaders[p.first] = s;
	}
	this->program = glCreateProgram();
	for (auto p : this->shaders) {
		glAttachShader(program, p.second);
	}
	glLinkProgram(program);
	GLint link;
	glGetProgramiv(program, GL_LINK_STATUS, &link);
	if (!link) {
		GLint l;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &l);
		string log;
		log.resize(l);
		glGetProgramInfoLog(program, l, NULL, &log[0]);
		throw std::exception(log.c_str());
	}
}

GLUtils::Shader::~Shader() {
	for (auto p : shaders) {
		glDeleteShader(p.second);
	}
	glDeleteProgram(program);
}