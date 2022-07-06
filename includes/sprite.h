#pragma once
#include "shaderprogram.h"
#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"


#include <glm/vec3.hpp>
#include <memory>


class Sprite {
public:
	enum class DrawType {
		Lines = 1,
		Triangles = 4
	};
	Sprite(const std::shared_ptr<ShaderProgram>& program);
	Sprite(const Sprite&) = delete;
	Sprite& operator=(const Sprite&) = delete;

	virtual void render(const glm::vec3& position, const glm::vec3& size, const glm::vec3& color, DrawType type = DrawType::Triangles) const;
	void set_program(const std::shared_ptr<ShaderProgram> program);
protected:
	mutable VertexArray m_VAO;
	VertexBuffer m_VBO;
	IndexBuffer m_EBO;
	std::shared_ptr<ShaderProgram> m_pProgram;
};