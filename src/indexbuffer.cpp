#include "indexbuffer.h"

IndexBuffer::IndexBuffer()
	: m_id(0)
	, m_indexes_count(0)
{
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_id);
}

void IndexBuffer::init(const void* data, std::size_t size, const VertexArray* const pVAO)
{
	if (m_id)
		return;
	if (pVAO)
		pVAO->bind();
	glGenBuffers(1, &m_id);
	bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	m_indexes_count = size / sizeof(GLuint);
}

void IndexBuffer::clear()
{
	glDeleteBuffers(1, &m_id);
	m_indexes_count = 0;
}

void IndexBuffer::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void IndexBuffer::unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GLuint IndexBuffer::get_indexes_count() const
{
	return m_indexes_count;
}
