#pragma once
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <string>
#include <array>
#include <memory>

class GameState;

class Game {
	enum class EGameState :uint8_t {
		StartScreen,
		Play,
		GameEnd
	};
public:
	Game(const glm::ivec2& window_size);
	
	void init();
	void render();
	void update(const float delta);
	
	void update_viewport();
	
	void set_button(const int button, const int action, const double xPos, const double yPos);
	void set_window_size(const glm::ivec2& window_size);

	bool check_win_condition()const;

	std::size_t get_current_width() const;
	std::size_t get_current_height() const;
private:
	std::array<bool, 7> m_buttons;
	glm::dvec2 m_cursor_position;

	glm::ivec2 m_window_size;
	glm::ivec4 m_viewport_vars;		//x, y, width, heigth 

	EGameState m_game_state_type;
	std::size_t m_level_index;
	std::shared_ptr<GameState> m_current_game_state = nullptr;
};