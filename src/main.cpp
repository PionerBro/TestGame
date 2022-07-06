#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <algorithm>
#include <chrono>
#include <memory>
#include <iostream>

#include "resourcemanager.h"
#include "game.h"


const glm::ivec2 W_START_SIZE = { 1024 ,768 };

std::unique_ptr<Game> g_game = std::make_unique<Game>(W_START_SIZE);

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_PRESS) {
		double xPos = 0.0;
		double yPos = 0.0;
		glfwGetCursorPos(window, &xPos, &yPos);
		g_game->set_button(button, action, xPos, yPos);
	}
}

void window_size_callback(GLFWwindow* window, int width, int height) {
	g_game->set_window_size(glm::ivec2(width, height));
}

int main(int argc, char** argv) {

	if (!glfwInit()) {
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* pWindow = glfwCreateWindow(W_START_SIZE.x, W_START_SIZE.y, "TestGame", nullptr, nullptr);
	if (!pWindow) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(pWindow);
	glfwSetWindowSizeCallback(pWindow, window_size_callback);
	glfwSetMouseButtonCallback(pWindow, mouse_button_callback);

	if (!gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress))) {
		glfwTerminate();
		return -1;
	}

	{
		ResourceManager::init(argv[0]);

		g_game->init();
		g_game->set_window_size(W_START_SIZE);

		glClearColor(1.f, 1.f, 1.f, 0.f);
		glEnable(GL_DEPTH_TEST);

		auto last_time = std::chrono::high_resolution_clock::now();

		while (!glfwWindowShouldClose(pWindow)) {
			auto current_time = std::chrono::high_resolution_clock::now();
			double duration = std::chrono::duration<double, std::milli>(current_time - last_time).count();
			if (duration < 10)
				continue;
			last_time = current_time;

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			g_game->update(duration);
			g_game->render();
			if (g_game->check_win_condition()) {
				std::cout << "You win\n";
				break;
			}
				
			glfwSwapBuffers(pWindow);
			glfwPollEvents();
		}
		g_game = nullptr;
		ResourceManager::unload_all_resources();
	}

	glfwTerminate();

	return 0;
}