#pragma once
#include <array>
#include <glm/vec2.hpp>

class GameState {
public:
	virtual ~GameState() = default;

	virtual void render() const = 0;
	virtual void update(const double delta) = 0;
	virtual void mouse_input(std::array<bool, 7>& buttons, const glm::dvec2& cursor_pos) {}

	virtual unsigned get_state_width() const = 0;
	virtual unsigned get_state_height() const = 0;
};