#include "connection.h"
#include "resourcemanager.h"
#include "sprite.h"

Connection::Connection(const glm::vec2& position, const glm::vec2& size, const glm::vec2& direction, const glm::vec3& color)
	: StaticObject(ResourceManager::get_sprite("resources/point"), position, size, direction), m_color(color)
{
}

void Connection::render() const
{
	m_pSprite->render(glm::vec3(m_position, 0.f), glm::vec3(m_size, 1.f), m_color);
}

void Connection::update(const double delta)
{
}
