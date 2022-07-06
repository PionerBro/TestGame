#pragma once
#include "staticobject.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>


class Connection : public StaticObject {
public:
	Connection(const glm::vec2& position, const glm::vec2& size, const glm::vec2& direction, const glm::vec3& color);
	virtual ~Connection() = default;

	virtual void render()const override;
	virtual void update(const double delta);
private:
	glm::vec3 m_color;
};