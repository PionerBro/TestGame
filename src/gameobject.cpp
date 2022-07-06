#include "gameobject.h"

GameObject::GameObject(const glm::vec2& position, const glm::vec2& size, const glm::vec2& direction)
	: m_position(position)
	, m_size(size)
	, m_direction(direction)
{
}

void GameObject::set_position(const glm::vec2 position)
{
	m_position = position;
}

void GameObject::set_size(const glm::vec2 size)
{
	m_size = size;
}

void GameObject::set_direction(const glm::vec2 direction)
{
	m_direction = direction;
}

const glm::vec2& GameObject::get_position() const&
{
	return m_position;
}

const glm::vec2& GameObject::get_size() const&
{
	return m_size;
}

const glm::vec2& GameObject::get_direction() const&
{
	return m_direction;
}
