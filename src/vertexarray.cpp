#include "vertexarray.h"
#include <numeric>
#include <glm/gtc/type_ptr.hpp>

VertexArray::VertexArray()
	: m_id(0)
	, m_attrib_count(0)
{
	glGenVertexArrays(1, &m_id);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_id);
}

void VertexArray::add_attrib_3fv(const glm::vec3& data)
{
	glEnableVertexAttribArray(m_attrib_count);
	glVertexAttrib3fv(m_attrib_count, glm::value_ptr(data));
	++m_attrib_count;
}

void VertexArray::set_attrib_3fv(const unsigned location, const glm::vec3& data)
{
	glEnableVertexAttribArray(location);
	glVertexAttrib3fv(location, glm::value_ptr(data));
}

void VertexArray::add_buffer(const VertexBuffer& buffer)
{
	bind();
	buffer.bind();
	decltype(auto) attribs_sizes = buffer.get_attribs_sizes();
	std::size_t attribs = attribs_sizes.size();
	int stride = std::accumulate(attribs_sizes.begin(), attribs_sizes.end(), 0);
	auto it = attribs_sizes.begin();
	for (std::size_t i = 0; i < attribs; ++i) {
		int data_pointer = std::accumulate(attribs_sizes.begin(), it, 0);
		glEnableVertexAttribArray(m_attrib_count);
		glVertexAttribPointer(m_attrib_count, attribs_sizes[i], GL_FLOAT, GL_FALSE, stride*sizeof(GL_FLOAT), (void*)(data_pointer*sizeof(GL_FLOAT)));
		++it;
		++m_attrib_count;
	}
}

void VertexArray::bind() const
{
	glBindVertexArray(m_id);
}

void VertexArray::unbind() const
{
	glBindVertexArray(0);
}

GLuint VertexArray::get_attrib_count() const
{
	return m_attrib_count;
}
