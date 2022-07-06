#pragma once
#include <glm/vec2.hpp>


class GameObject {
public:
	GameObject(const glm::vec2& position, const glm::vec2& size, const glm::vec2& direction);
	virtual ~GameObject() = default;
	GameObject(const GameObject& other) = default;
	GameObject& operator=(const GameObject& other) = default;

	virtual void render()const = 0;
	virtual void update(const double delta) = 0;

	void set_position(const glm::vec2 position);
	void set_size(const glm::vec2 size);
	void set_direction(const glm::vec2 direction);
	const glm::vec2& get_position()const&;
	const glm::vec2& get_size()const&;
	const glm::vec2& get_direction()const&;
protected:
	glm::vec2 m_position{};
	glm::vec2 m_size{};
	glm::vec2 m_direction{};
};