#include "vertexbuffer.h"

VertexBuffer::VertexBuffer()
	: m_data_size(0)
	, m_id(0)
{
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_id);
}

void VertexBuffer::init(const void* data, std::size_t size, const std::vector<unsigned>& attribs_sizes)
{
	if (m_id)
		return;
	glGenBuffers(1, &m_id);
	bind();
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	m_attribs_sizes = attribs_sizes;
	m_data_size = size;
}

void VertexBuffer::update(const void* data, std::size_t size)
{
	glGenBuffers(1, &m_id);
	bind();
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

void VertexBuffer::clear()
{
	glDeleteBuffers(1, &m_id);
	m_attribs_sizes.clear();
}

void VertexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void VertexBuffer::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

const std::vector<unsigned>& VertexBuffer::get_attribs_sizes() const&
{
	return m_attribs_sizes;
}

std::size_t VertexBuffer::get_data_size() const
{
	return m_data_size;
}
