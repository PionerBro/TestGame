#pragma once
#include "gamestate.h"

#include <glm/vec3.hpp>

#include <vector>
#include <memory>
#include <string>
#include <stack>

class Chip;
class Point;
class Connection;

class Level : public GameState {
public:
	Level(const std::string& description);

	virtual void render()const override;
	virtual void update(const double delta) override;
	virtual void mouse_input(std::array<bool, 7>& buttons, const glm::dvec2& cursor_pos) override;

	virtual unsigned get_state_width() const override;
	virtual unsigned get_state_height() const override;

	bool check_win_condition() const;
private:
	void parse_level_description(const std::string& description);
	void btn_press_handler(const glm::vec2& cursor_position);
	void way_clear();

	glm::uvec2 to_uvec2(const std::string& str);
	std::vector<int> to_vector_int(const std::string& str);
	glm::uvec3 rows_count_min_max(const std::vector<glm::uvec2>& vec);
	glm::uvec3 columns_count_min_max(const std::vector<glm::uvec2>& vec);
	unsigned connection_direction(const glm::uvec2& first, const glm::uvec2& second);

	std::stack<Point*> make_way(Point* start, Point* finish);
	glm::vec3 get_color(unsigned number, unsigned count);
private:
	std::vector<std::shared_ptr<Point>> m_points;
	std::vector<std::shared_ptr<Point>> m_hint_points;
	std::vector<std::shared_ptr<Connection>> m_connections;
	std::vector<std::shared_ptr<Chip>> m_chips;
	std::vector<std::shared_ptr<Chip>> m_hint_chips;
	

	Chip* m_active_chip = nullptr;
	bool m_input_off = false;

	unsigned m_state_width = 0;
	unsigned m_state_height = 0;
};