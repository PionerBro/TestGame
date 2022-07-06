#include "shaderprogram.h"

#include <glm/gtc/type_ptr.hpp>

#include <iostream>

ShaderProgram::ShaderProgram(const std::string& vertex_source, const std::string& fragment_source)
	: m_id(0)
	, m_bIs_compiled(false)
{
	decltype(auto) v_source = vertex_source.c_str();
	decltype(auto) f_source = fragment_source.c_str();

	GLuint v_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(v_shader, 1, &v_source, nullptr);
	glCompileShader(v_shader);
	GLint succes = 0;
	GLchar infoLog[512];
	glGetShaderiv(v_shader, GL_COMPILE_STATUS, &succes);
	if (!succes) {
		glGetShaderInfoLog(v_shader, 512, nullptr, infoLog);
		std::cerr << "VERTEX SHADER ERROR " << infoLog << "\n";
		glDeleteShader(v_shader);
		return;
	}

	GLuint f_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(f_shader, 1, &f_source, nullptr);
	glCompileShader(f_shader);
	glGetShaderiv(f_shader, GL_COMPILE_STATUS, &succes);
	if (!succes) {
		glGetShaderInfoLog(f_shader, 512, nullptr, infoLog);
		std::cerr << "FRAGMENT SHADER ERROR " << infoLog << "\n";
		glDeleteShader(v_shader);
		glDeleteShader(f_shader);
		return;
	}

	m_id = glCreateProgram();
	glAttachShader(m_id, v_shader);
	glAttachShader(m_id, f_shader);
	glLinkProgram(m_id);
	glGetProgramiv(m_id, GL_LINK_STATUS, &succes);
	if (!succes) {
		glGetProgramInfoLog(m_id, 512, nullptr, infoLog);
		std::cerr << "SHADER PROGRAM ERROR " << infoLog << "\n";
	}
	else {
		m_bIs_compiled = true;
	}
	glDetachShader(m_id, v_shader);
	glDetachShader(m_id, f_shader);
	glDeleteShader(v_shader);
	glDeleteShader(f_shader);
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_id);
}

void ShaderProgram::use() const
{
	glUseProgram(m_id);
}

bool ShaderProgram::is_compiled() const
{
	return m_bIs_compiled;
}

void ShaderProgram::set_matrix4fv(const std::string& name, const glm::mat4& value)
{
	glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}