#include "point.h"
#include "resourcemanager.h"
#include "chip.h"
#include "sprite.h"

static const double g_timer_duration = 500.0;

Point::Point(const glm::vec2& position, const glm::vec2& size, const glm::vec2& direction, const glm::vec3& color, const glm::vec3& light_color)
	: StaticObject(ResourceManager::get_sprite("resources/point"),position, size, direction)
	, m_chip(nullptr)
	, m_light_color(light_color)
	, m_color(m_light_color)
	, m_main_color(color)
	, m_active(false)
{
	static unsigned id = 0;
	m_id = id;
	++id;

	m_color_timer.set_callback([&]() {
		m_color = (m_color == m_main_color) ? m_light_color : m_main_color;
		m_color_timer.start(g_timer_duration);
	});
}

void Point::render() const
{
	if(m_active)
		m_pSprite->render(glm::vec3(m_position, 0.f), glm::vec3(m_size, 1.f), m_color);
	else
		m_pSprite->render(glm::vec3(m_position, 0.f), glm::vec3(m_size, 1.f), m_main_color);

}

void Point::update(const double delta)
{
	m_color_timer.update(delta);
}

void Point::set_light_color(const glm::vec3& color)
{
	m_light_color = color;
}

const glm::vec3& Point::get_light_color() const&
{
	return m_light_color;
}

unsigned Point::get_id() const
{
	return m_id;
}

void Point::choice_hint_on() 
{
	for (const auto& neighbor : m_neighbors) {
		if (neighbor->m_active || neighbor->m_chip)
			continue;
		neighbor->m_active = true;
		neighbor->m_color = m_light_color;
		neighbor->m_color_timer.start(g_timer_duration);
		neighbor->choice_hint_on();
	}
}

void Point::choice_hint_off()
{
	for (const auto& neighbor : m_neighbors) {
		if (!neighbor->m_active)
			continue;
		neighbor->m_active = false;
		neighbor->m_color_timer.stop();
		neighbor->choice_hint_off();
	}
}

void Point::add_neighbor(Point* neighbor)
{
	m_neighbors.push_back(neighbor);
}

void Point::set_chip(Chip* chip)
{
	m_chip = chip;
}

Chip* Point::get_chip()
{
	return m_chip;
}

bool Point::is_active() const
{
	return m_active;
}

std::vector<Point*>& Point::get_neighbors()
{
	return m_neighbors;
}

unsigned Point::get_way_costs() const
{
	return m_way_costs;
}

void Point::set_way_costs(unsigned way_costs)
{
	m_way_costs = way_costs;
}


