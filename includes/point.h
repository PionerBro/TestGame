#pragma once
#include "staticobject.h"
#include "timer.h"

#include <stack>

class Chip;

class Point : public StaticObject {
public:
	Point(const glm::vec2& position, const glm::vec2& size, const glm::vec2& direction, const glm::vec3& color, const glm::vec3& light_color);
	virtual ~Point() = default;

	virtual void render()const override;
	virtual void update(const double delta)override;

	void set_light_color(const glm::vec3& color);
	void choice_hint_on();
	void choice_hint_off();
	void add_neighbor(Point* neighbor);
	void set_chip(Chip* chip);
	void set_way_costs(unsigned way_costs);

	const glm::vec3& get_light_color()const&;
	std::vector<Point*>& get_neighbors();
	Chip* get_chip();
	unsigned get_way_costs()const;
	unsigned get_id()const;
	bool is_active()const;
private:
	std::vector<Point*> m_neighbors;
	Chip* m_chip;
	Timer m_color_timer;

	glm::vec3 m_light_color{};
	glm::vec3 m_color{};
	glm::vec3 m_main_color;
	bool m_active;
	unsigned m_id;
	unsigned m_way_costs = 0;
};