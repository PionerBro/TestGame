#pragma once
#include <glad/glad.h>
#include <glm/mat4x4.hpp>

#include <string>

class ShaderProgram {
public:
	ShaderProgram(const std::string& vertex_source, const std::string& fragment_source);
	~ShaderProgram();
	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram& operator=(const ShaderProgram&) = delete;


	void use() const;
	bool is_compiled()const;
	void set_matrix4fv(const std::string& name, const glm::mat4& value);
private:
	GLuint m_id = 0;
	bool  m_bIs_compiled = false;
};