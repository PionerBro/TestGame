#pragma once
#include "vertexarray.h"

#include <glad/glad.h>

#include <vector>
#include <memory>

class IndexBuffer {
public:
	IndexBuffer();
	~IndexBuffer();

	IndexBuffer(const IndexBuffer&) = delete;
	IndexBuffer& operator=(const IndexBuffer&) = delete;

	void init(const void* data, std::size_t size, const VertexArray* const pVAO = nullptr);
	void clear();
	void bind() const;
	void unbind() const;
	GLuint get_indexes_count()const;
private:
	GLuint m_id;
	GLuint m_indexes_count;
};