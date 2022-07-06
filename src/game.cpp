#include "game.h"
#include "gamestate.h"
#include "resourcemanager.h"
#include "level.h"
#include "shaderprogram.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

Game::Game(const glm::ivec2& window_size)
	: m_window_size(window_size)
	, m_game_state_type(EGameState::Play)
	, m_level_index(0)
	, m_cursor_position(0,0)
	, m_viewport_vars(0,0,window_size.x, window_size.y)
{
	m_buttons.fill(false);
}

void Game::init()
{
	ResourceManager::load_all_resources("resources/resources.txt");
	m_current_game_state = std::make_shared<Level>(ResourceManager::get_level("resources/level"));
	set_window_size(m_window_size);
}
void Game::render()
{
	m_current_game_state->render();
}

void Game::update(const float delta)
{
	m_current_game_state->mouse_input(m_buttons, m_cursor_position);
	m_current_game_state->update(delta);
}

void Game::set_button(const int button, const int action, const double xPos, const double yPos)
{
	m_buttons[button] = action;
	m_cursor_position.x = (xPos - m_viewport_vars.x)/(static_cast<float>(m_viewport_vars.z)/get_current_width());
	m_cursor_position.y = (yPos - m_viewport_vars.y) / (static_cast<float>(m_viewport_vars.w) / get_current_height());
}

void Game::set_window_size(const glm::ivec2& window_size)
{
	m_window_size = window_size;
	update_viewport();
}

bool Game::check_win_condition() const
{
	if (m_game_state_type == EGameState::Play)
		return static_cast<Level*>(m_current_game_state.get())->check_win_condition();
	return false;
}

std::size_t Game::get_current_width() const
{
	return m_current_game_state->get_state_width();
}

std::size_t Game::get_current_height() const
{
	return m_current_game_state->get_state_height();
}

void Game::update_viewport()
{
	const float aspect_ratio = static_cast<float>(get_current_width()) / get_current_height();
	unsigned viewport_width = m_window_size.x;
	unsigned viewport_height = m_window_size.y;
	unsigned viewport_left_offset = 0;
	unsigned viewport_bottom_offset = 0;

	if (static_cast<float>(viewport_width) / viewport_height > aspect_ratio) {
		viewport_width = static_cast<unsigned>(viewport_height * aspect_ratio);
		viewport_left_offset = (m_window_size.x - viewport_width) / 2;
	}
	else {
		viewport_height = static_cast<unsigned>(viewport_width / aspect_ratio);
		viewport_bottom_offset = (m_window_size.y - viewport_height) / 2;
	}

	m_viewport_vars.x = viewport_left_offset;
	m_viewport_vars.y = viewport_bottom_offset;
	m_viewport_vars.z = viewport_width;
	m_viewport_vars.w = viewport_height;

	glm::mat4 projection = glm::ortho(0.f, static_cast<float>(get_current_width()), 0.f, static_cast<float>(get_current_height()), -100.f, 100.f);
	glViewport(viewport_left_offset, viewport_bottom_offset, viewport_width, viewport_height);

	auto pr = ResourceManager::get_shader("resources/shader");
	pr->use();
	pr->set_matrix4fv("projection", projection);
}
