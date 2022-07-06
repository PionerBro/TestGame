#include "chip.h"
#include "resourcemanager.h"
#include "point.h"

Chip::Chip(const glm::vec2& position, const glm::vec2& size, const glm::vec2& direction, const glm::vec3& color, const unsigned win_point_id)
	: StaticObject(ResourceManager::get_sprite("resources/point"), position, size, direction)
	, m_point(nullptr)
	, m_color(color)
	, m_win_point_id(win_point_id)
	, m_target_position(position)
{
}

void Chip::render() const
{
	if (way_is_finish()) {
		if (m_active) {
			m_pSprite->render(glm::vec3(m_position, 0.f), glm::vec3(m_size, 1.f), m_point->get_light_color(), Sprite::DrawType::Lines);
		}
		m_pSprite->render(glm::vec3(m_position, 0.f), glm::vec3(m_size, 1.f), m_color);
	}
	else {
		m_pSprite->render(glm::vec3(m_target_position, 0.f), glm::vec3(m_size, 1.f), m_color);
	}
}

void Chip::update(const double delta)
{
	const float speed = 5.f;
	if (way_is_finish())
		return;
	Direction direction = get_way_direction(m_position, m_way.top()->get_position());
	float remainder = 0.f;
	if (direction == Direction::Bottom) {
		m_target_position.y -= speed;
		if (m_target_position.y <= m_way.top()->get_position().y) {
			remainder = m_way.top()->get_position().y - m_target_position.y;
			m_position = m_target_position = m_way.top()->get_position();
			m_way.pop();
		}
	}
	else if (direction == Direction::Top) {
		m_target_position.y += speed;
		if (m_target_position.y >= m_way.top()->get_position().y) {
			remainder = m_target_position.y - m_way.top()->get_position().y;
			m_position = m_target_position = m_way.top()->get_position();
			m_way.pop();
		}
	}
	else if (direction == Direction::Left) {
		m_target_position.x -= speed;
		if (m_target_position.x <= m_way.top()->get_position().x) {
			remainder = m_way.top()->get_position().x - m_target_position.x;
			m_position = m_target_position = m_way.top()->get_position();
			m_way.pop();
		}
	}
	else {
		m_target_position.x += speed;
		if (m_target_position.x >= m_way.top()->get_position().x) {
			remainder = m_target_position.x - m_way.top()->get_position().x;
			m_position = m_target_position = m_way.top()->get_position();
			m_way.pop();
		}
	}

	if (remainder > 0.f && !m_way.empty()) {
		Direction direction = get_way_direction(m_position, m_way.top()->get_position());
		if (direction == Direction::Bottom)
			m_target_position.y -= speed;
		else if (direction == Direction::Top)
			m_target_position.y += speed;
		else if (direction == Direction::Left) 
			m_target_position.x -= speed;
		else 
			m_target_position.x += speed;
	}
}

void Chip::set_point(Point* point)
{
	m_point = point;
}

Point* Chip::get_point()
{
	return m_point;
}

unsigned Chip::get_win_point_id() const
{
	return m_win_point_id;
}

void Chip::set_active(bool active)
{
	m_active = active;
}

Chip::Direction Chip::get_way_direction(const glm::vec2& first, const glm::vec2& second)
{
	if (first.x == second.x)
		return (first.y > second.y) ? Direction::Bottom : Direction::Top;
	else
		return (first.x > second.x) ? Direction::Left : Direction::Right;
}

void Chip::set_way(std::stack<Point*>&& way)
{
	m_way = std::move(way);
}

bool Chip::way_is_finish() const
{
	return m_way.empty();
}