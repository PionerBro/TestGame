#pragma once

#include <glad/glad.h>
#include <vector>

class VertexBuffer {
public:
	VertexBuffer();
	~VertexBuffer();

	VertexBuffer(const VertexBuffer&) = delete;
	VertexBuffer& operator=(const VertexBuffer&) = delete;
	
	void init(const void* data, std::size_t size, const std::vector<unsigned>& attribs_sizes);
	void update(const void* data, std::size_t size);
	void clear();
	void bind() const;
	void unbind() const;

	const std::vector<unsigned>& get_attribs_sizes()const&;
	std::size_t get_data_size()const;
private:
	std::vector<unsigned> m_attribs_sizes;
	std::size_t m_data_size;
	GLuint m_id;
};