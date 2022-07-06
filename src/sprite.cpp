#include "sprite.h"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <numeric>


Sprite::Sprite(const std::shared_ptr<ShaderProgram>& program)
	: m_pProgram(program)
{
	GLfloat data[] = {
	  0.f, 0.f, 0.0f,
	  0.f, 1.f, 0.0f,
	  1.f, 0.f, 0.0f,
	  1.f, 1.f, 0.0f,

	  0.f, 0.f, 0.f,
	  1.f, 0.f, 0.f,
	  0.f, 1.f, 0.f,
	  1.f, 1.f, 0.f
	};

	GLuint indexes[] = {
		0, 1, 3,
		0, 2, 3,
	};

	m_VBO.init(data, sizeof(data), {3});
	m_VAO.add_buffer(m_VBO);
	m_EBO.init(indexes, sizeof(indexes), &m_VAO);
}

void Sprite::render(const glm::vec3& position, const glm::vec3& size, const glm::vec3& color, DrawType type) const
{
	glm::mat4 model(1.f);
	model = glm::translate(model, position);
	model = glm::scale(model, size);

	m_pProgram->use();
	m_pProgram->set_matrix4fv("model", model);
	m_VAO.bind();
	m_VAO.set_attrib_3fv(1, color);

	switch (type)
	{
	case Sprite::DrawType::Lines:
		glDrawArrays(GL_LINES, 0, m_VBO.get_data_size() / std::accumulate(m_VBO.get_attribs_sizes().begin(), m_VBO.get_attribs_sizes().end(), 0));
		break;
	case Sprite::DrawType::Triangles:
		glDrawElements(GL_TRIANGLES, m_EBO.get_indexes_count(), GL_UNSIGNED_INT, nullptr);
		break;
	}
	
}

void Sprite::set_program(const std::shared_ptr<ShaderProgram> program)
{
	m_pProgram = program;
}
