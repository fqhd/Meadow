#include "Window.hpp"
#include "InputManager.hpp"
#include "GUIRenderer.hpp"
#include "Game.hpp"
#include "GameState.hpp"
#include "MainMenu.hpp"
#include "PauseMenu.hpp"

int main() {

	Window::create();
	InputManager::init(Window::getWindowPtr());
	GUIRenderer::init(WINDOW_WIDTH, WINDOW_HEIGHT);

	Game game;
	game.init();
	PauseMenu pausemenu;
	pausemenu.init();
	MainMenu mainmenu;
	mainmenu.init();
	GameState gamestate = GameState::MainMenu;


	double before = glfwGetTime();
	while (gamestate != GameState::Exit) {
		Window::clear();
		double dt = glfwGetTime() - before;
		before = glfwGetTime();
		if (!InputManager::processInput()) gamestate = GameState::Exit;

		if (gamestate == GameState::Game) {
			game.update(dt);
			game.render();
		}
		else if (gamestate == GameState::MainMenu) {
			mainmenu.update(dt);
			mainmenu.render();
		}
		else if (gamestate == GameState::Pause) {
			pausemenu.update();
			pausemenu.render();
		}

		GUIRenderer::render();

		Window::update();
	}

	game.destroy();
	mainmenu.destroy();
	pausemenu.destroy();
	Window::close();

	return 0;
}