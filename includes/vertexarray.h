#pragma once
#include "vertexbuffer.h"

#include <glad/glad.h>
#include <glm/vec3.hpp>


class VertexArray {
public:
	VertexArray();
	~VertexArray();

	VertexArray(const VertexArray&) = delete;
	VertexArray& operator=(const VertexArray&) = delete;

	void add_attrib_3fv(const glm::vec3& data);
	void set_attrib_3fv(const unsigned location, const glm::vec3& data);
	void add_buffer(const VertexBuffer& buffer);
	void bind() const ;
	void unbind() const ;
	GLuint get_attrib_count()const;
	
private:
	GLuint m_id;
	GLuint m_attrib_count;
};
