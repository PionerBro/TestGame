#include "level.h"
#include "point.h"
#include "chip.h"
#include "connection.h"
#include "inputcodes.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <charconv>
#include <sstream>
#include <algorithm>

constexpr float G_WIN_MAP_SCALE = 0.25f;

Level::Level(const std::string& description)
{
	parse_level_description(description);
}

void Level::render() const
{
	for (const auto& chip : m_chips)
		chip->render();
	for (const auto& chip : m_hint_chips)
		chip->render();
	for (const auto& point : m_points)
		point->render();
	for (const auto& point : m_hint_points)
		point->render();
	for (const auto& con : m_connections)
		con->render();
	
}

void Level::update(const double delta)
{
	for (const auto& chip : m_chips)
		chip->update(delta);
	for (const auto& point : m_points)
		point->update(delta);
}

void Level::mouse_input(std::array<bool, 7>& buttons, const glm::dvec2& cursor_pos)
{
	if (buttons[static_cast<std::size_t>(MouseButtonCode::BUTTON_LEFT)]) {
		buttons[static_cast<std::size_t>(MouseButtonCode::BUTTON_LEFT)] = false;
		btn_press_handler(cursor_pos);
	}
}

unsigned Level::get_state_width() const
{
	return m_state_width;
}

unsigned Level::get_state_height() const
{
	return m_state_height;
}

bool Level::check_win_condition() const
{
	for (const auto& chip : m_chips) {
		if (chip->get_win_point_id() != chip->get_point()->get_id())
			return false;
	}
	return true;
}

void Level::parse_level_description(const std::string& description)
{
	std::stringstream level_data(description);
	unsigned chips_count = 0, points_count = 0, connection_count = 0;

	std::string tmp;
	std::getline(level_data, tmp);
	std::from_chars(tmp.c_str(), tmp.c_str() + tmp.size(), chips_count);

	std::getline(level_data, tmp);
	std::from_chars(tmp.c_str(), tmp.c_str() + tmp.size(), points_count);

	std::vector<glm::uvec2> points_coords(points_count);
	
	for (unsigned i = 0; i < points_count; ++i) {
		std::getline(level_data, tmp);
		points_coords[i] = to_uvec2(tmp);
	}
	glm::uvec3 rows = rows_count_min_max(points_coords);
	glm::uvec3 columns = columns_count_min_max(points_coords);

	unsigned point_width = (rows.z - rows.y) / ((rows.x - 1) << 1);
	unsigned point_height = (columns.z - columns.y) / ((columns.x - 1) << 1);

	glm::vec2 size = (point_width < point_height) ? glm::vec2{ (float)point_width } : glm::vec2{ (float)point_height };

	m_state_width = (rows.z + size.x + rows.y) * (1+ G_WIN_MAP_SCALE);
	m_state_height = columns.z + size.x + columns.y;

	glm::vec2 additional_pos = { (rows.z + size.x + rows.y) * G_WIN_MAP_SCALE, 0.f };

	m_points.resize(points_count);
	m_hint_points.resize(points_count);

	glm::vec3 way_color = { 0.5f, 0.5f, 0.5f };
	glm::vec3 light_color = { 1.0f,1.0f, 0.0f };
	for (unsigned i = 0; i < points_count; ++i) {
		m_points[i] = std::make_shared<Point>(glm::vec2(points_coords[i]) + additional_pos, 
			size, glm::vec2{ 0.f }, way_color, light_color);
	}

	for (unsigned i = 0; i < points_count; ++i) {
		m_hint_points[i] = std::make_shared<Point>(glm::vec2(points_coords[i]) * G_WIN_MAP_SCALE,
			size * G_WIN_MAP_SCALE, glm::vec2{ 0.f }, way_color, light_color);
	}

	std::getline(level_data, tmp);
	std::vector<int> start_chips_position = to_vector_int(tmp);

	std::getline(level_data, tmp);
	std::vector<int> win_chips_position = to_vector_int(tmp);

	m_chips.resize(chips_count);
	m_hint_chips.resize(chips_count);

	for (std::size_t i = 0; i < chips_count; ++i) {
		unsigned start_position_index = start_chips_position[i] - 1;
		unsigned end_position_index = win_chips_position[i] - 1;


		glm::vec2 start_position = points_coords[start_position_index];
		glm::vec2 end_position = points_coords[end_position_index];
		glm::vec3 color = get_color(i, chips_count);
		m_chips[i] = std::make_shared<Chip>(start_position + additional_pos,
			size,
			glm::vec2{ 0.f },
			color,
			end_position_index);


		m_points[start_position_index]->set_chip(m_chips[i].get());
		m_chips[i]->set_point(m_points[start_position_index].get());


		m_hint_chips[i] = std::make_shared<Chip>(end_position * G_WIN_MAP_SCALE,
			size * G_WIN_MAP_SCALE,
			glm::vec2{ 0.f },
			color,
			end_position_index);		
	}

	std::getline(level_data, tmp);
	std::from_chars(tmp.c_str(), tmp.c_str() + tmp.size(), connection_count);

	m_connections.resize(static_cast<std::size_t>(connection_count) << 1);
	for (std::size_t i = 0; i < connection_count; ++i) {
		std::getline(level_data, tmp);
		glm::uvec2 connection = to_uvec2(tmp) - 1u;
		m_points[connection.x]->add_neighbor(m_points[connection.y].get());
		m_points[connection.y]->add_neighbor(m_points[connection.x].get());
		glm::vec4 connection_pos = {};
		unsigned direction = connection_direction(points_coords[connection.x], points_coords[connection.y]);
		if (direction == 0) {
			connection_pos = { points_coords[connection.x].x + size.x / 4.f, points_coords[connection.x].y + size.x, points_coords[connection.y].x + size.x * 3.f / 4.f, points_coords[connection.y].y };
		}
		else if (direction == 1) {
			connection_pos = { points_coords[connection.y].x + size.x / 4.f, points_coords[connection.y].y + size.x, points_coords[connection.x].x + size.x * 3.f / 4.f, points_coords[connection.x].y };
		}
		else if (direction == 2) {
			connection_pos = { points_coords[connection.x].x + size.x, points_coords[connection.x].y + size.x / 4.f, points_coords[connection.y].x, points_coords[connection.y].y + size.x * 3.f / 4.f };
		}
		else {
			connection_pos = { points_coords[connection.y].x, points_coords[connection.y].y + size.x / 4.f, points_coords[connection.x].x + size.x, points_coords[connection.x].y + size.x * 3.f / 4.f };
		}
		m_connections[i] = std::make_shared<Connection>(glm::vec2(connection_pos.x, connection_pos.y) + additional_pos,
			glm::vec2(connection_pos.z - connection_pos.x, connection_pos.w - connection_pos.y),
			glm::vec2{ 0.f },
			way_color);
		m_connections[i + connection_count] = std::make_shared<Connection>(glm::vec2(connection_pos.x, connection_pos.y) * G_WIN_MAP_SCALE,
			glm::vec2(connection_pos.z - connection_pos.x, connection_pos.w - connection_pos.y) * G_WIN_MAP_SCALE,
			glm::vec2{ 0.f },
			way_color);
	}
}

void Level::btn_press_handler(const glm::vec2& cursor_position)
{
	if (m_input_off){
		if (!m_active_chip->way_is_finish())
			return;
		m_active_chip = nullptr;
		m_input_off = false;
	}
	for (const auto& point : m_points) {
		unsigned point_x = point->get_position().x;
		unsigned point_y = point->get_position().y;
		unsigned pos_x = cursor_position.x;
		unsigned pos_y = cursor_position.y;

		if (cursor_position.x >= point->get_position().x && cursor_position.x <= (point->get_position().x + point->get_size().x) &&
			cursor_position.y <= m_state_height - point->get_position().y && cursor_position.y >= (m_state_height - point->get_position().y - point->get_size().y))
		{
			if (!m_active_chip && point->get_chip()) {
				m_active_chip = point->get_chip();
				m_active_chip->set_active(true);
				point->choice_hint_on();
			}
			else {
				if (m_active_chip) {
					if (m_active_chip->get_point()->get_id() == point->get_id()) {
						m_active_chip->get_point()->choice_hint_off();
						m_active_chip->set_active(false);
						m_active_chip = nullptr;
					}
					else if (point->is_active()) {
						m_active_chip->set_way(make_way(m_active_chip->get_point(), point.get()));
						m_active_chip->get_point()->choice_hint_off();
						m_active_chip->get_point()->set_chip(nullptr);
						point->set_chip(m_active_chip);
						m_active_chip->set_point(point.get());
						way_clear();
						m_input_off = true;
						m_active_chip->set_active(false);
					}
				}
			}
			break;
		}
	}
}

std::stack<Point*> Level::make_way(Point* start, Point* finish) {
	std::stack<Point*> stck;
	Point* next = nullptr;
	Point* cur;
	Point* prev = cur = start;
	while (true) {
		if (cur->get_way_costs() == 0)
			cur->set_way_costs(prev->get_way_costs() + 1);
		for (auto& neighbor : cur->get_neighbors()) {
			if (neighbor->is_active()) {
				if (neighbor->get_way_costs() == 0) {
					if (!next)
						next = neighbor;
					else {
						stck.push(neighbor);
						neighbor->set_way_costs(cur->get_way_costs() + 1);
					}
				}
				else if (neighbor->get_way_costs() > cur->get_way_costs() + 1) {
					neighbor->set_way_costs(cur->get_way_costs() + 1);
					if (!next)
						next = neighbor;
					else
						stck.push(neighbor);
				}
				else if (neighbor->get_way_costs() + 1 < cur->get_way_costs()) {
					cur->set_way_costs(neighbor->get_way_costs() + 1);
					if (!next)
						next = cur;
					else
						stck.push(neighbor);
				}
			}
		}
		if (!next) {
			if (stck.empty())
				break;
			next = stck.top();
			stck.pop();
		}
		prev = cur;
		cur = next;
		next = nullptr;
	}

	cur = finish;
	while (cur != start) {
		for (const auto& neighbor : cur->get_neighbors()) {
			if (neighbor->get_way_costs() == cur->get_way_costs() - 1) {
				stck.push(cur);
				cur = neighbor;
				break;
			}
		}
	}
	return stck;
}

void Level::way_clear()
{
	for (const auto& point : m_points)
		point.get()->set_way_costs(0);
}

glm::vec3 Level::get_color(unsigned number, unsigned count)
{
	static constexpr glm::vec3 Blue		= {   0,  0,255 };
	static constexpr glm::vec3 Aqua		= {	  0,255,255 };
	static constexpr glm::vec3 Lime		= {   0,255,  0 };
	static constexpr glm::vec3 Red		= { 255,  0,  0 };
	static constexpr glm::vec3 Fuchsia  = { 255,  0,255 };

	static const std::vector<glm::uvec3> colors =  {
		{   0,   0, 255 },
		{	0, 255, 255 },
		{   0, 255,   0 },
		{ 255,   0,   0 },
		{ 255,   0, 255 }
	};
	static const std::size_t color_size = colors.size();

	unsigned tone_count = count / color_size;
	unsigned remainder = count % color_size;
	if (remainder)
		++tone_count;
	unsigned tone = number / color_size;
	unsigned color = number % color_size;

	glm::uvec3 result = colors[color];

	result.r = (result.r) ? (127 * ((static_cast<float>(tone)) / tone_count) + 128) : (127 * ((static_cast<float>(tone)) / tone_count));
	result.g = (result.g) ? (127 * ((static_cast<float>(tone)) / tone_count) + 128) : (127 * ((static_cast<float>(tone)) / tone_count));
	result.b = (result.b) ? (127 * ((static_cast<float>(tone)) / tone_count) + 128) : (127 * ((static_cast<float>(tone)) / tone_count));
	
	return glm::vec3(result)/255.f;
}


glm::uvec2 Level::to_uvec2(const std::string& str)
{
	glm::vec2 result{ 0 };
	if (str.empty())
		return result;
	std::size_t pos = str.find(',');
	if (pos == std::string::npos)
		return result;

	unsigned x = 0;
	unsigned y = 0;
	auto conv_result = std::from_chars(str.c_str(), str.c_str() + pos, x);
	if (conv_result.ec != std::errc())
		return result;
	conv_result = std::from_chars(str.c_str() + pos + 1, str.c_str() + str.size(), y);
	if (conv_result.ec != std::errc())
		return result;

	result = { x, y };
	return result;
}

std::vector<int> Level::to_vector_int(const std::string& str)
{
	std::vector<int> vec;
	int value = 0;
	std::size_t pos = 0;
	auto it = str.rbegin();
	while (it != str.rend()) {
		char c = *it;
		if (std::isdigit(c)) {
			value += std::pow(10, pos) * (c - '0');
			++pos;
		}
		else if (c == ',') {
			vec.push_back(value);
			value = 0;
			pos = 0;
		}
		++it;
	}
	vec.push_back(value);
	std::reverse(vec.begin(), vec.end());
	return vec;
}

glm::uvec3 Level::rows_count_min_max(const std::vector<glm::uvec2>& vec)
{
	if (vec.empty())
		return { 0, 0 ,0 };
	std::vector<unsigned> tmp(vec.size());
	std::size_t i = 0;
	for (const auto& v : vec) {
		tmp[i] = v.x;
		++i;
	}
	std::sort(tmp.begin(), tmp.end());
	unsigned value = 0;
	unsigned count = 0;
	unsigned max_count = 0;
	for (const auto& v : tmp) {
		if (v != value) {
			if (count > max_count)
				max_count = count;
			count = 0;
			value = v;
		}
		++count;
	}

	return { max_count, tmp[0], value};
}

glm::uvec3 Level::columns_count_min_max(const std::vector<glm::uvec2>& vec)
{
	if (vec.empty())
		return { 0,0,0 };
	std::vector<unsigned> tmp(vec.size());
	std::size_t i = 0;
	for (const auto& v : vec) {
		tmp[i] = v.y;
		++i;
	}
	std::sort(tmp.begin(), tmp.end());
	unsigned value = 0;
	unsigned count = 0;
	unsigned max_count = 0;
	for (const auto& v : tmp) {
		if (v != value) {
			if (count > max_count)
				max_count = count;
			count = 0;
			value = v;
		}
		++count;
	}
	return { max_count, tmp[0], value };
}

unsigned Level::connection_direction(const glm::uvec2& first, const glm::uvec2& second)
{
	if (first.y < second.y) {			//top
		return 0;
	}
	else if (first.y > second.y) {		//bottom
		return 1;
	}
	else {
		if (first.x < second.x)			//right
			return 2;
		else
			return 3;					//left
	}
}