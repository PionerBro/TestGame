#pragma once
#include "staticobject.h"
#include "sprite.h"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include <stack>

class Point;

class Chip : StaticObject {
	enum class Direction : uint8_t{
		Top,
		Bottom,
		Left,
		Right
	};
public:
	Chip(const glm::vec2& position, const glm::vec2& size, const glm::vec2& direction, const glm::vec3 & color, const unsigned win_point_id);
	virtual ~Chip() = default;

	virtual void render()const override;
	virtual void update(const double delta)override;

	void set_way(std::stack<Point*>&& way);
	bool way_is_finish() const;
	void set_point(Point* point);
	Point* get_point();
	unsigned get_win_point_id()const;
	void set_active(bool active);


	static Direction get_way_direction(const glm::vec2& first, const glm::vec2& second);
private:
	Point* m_point;
	glm::vec3 m_color;
	unsigned m_win_point_id;
	glm::vec2 m_target_position;

	bool m_active = false;
	std::stack<Point*> m_way;
};