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
	pausemenu.init(&game.world);
	MainMenu mainmenu;
	mainmenu.init(&game);
	GameState gamestate = GameState::MainMenu;

	double before = glfwGetTime();
	while (gamestate != GameState::Exit) {
		Window::clear();
		double nowTime = glfwGetTime();
		double dt = nowTime - before;
		before = nowTime;
		if (!InputManager::processInput()) gamestate = GameState::Exit;

		if (gamestate == GameState::Game) {
			game.update(dt, gamestate);
			game.render();
		}
		else if (gamestate == GameState::MainMenu) {
			mainmenu.update(dt, gamestate);
			mainmenu.render();
		}
		else if (gamestate == GameState::Pause) {
			pausemenu.update(dt, gamestate);
			game.render();
			pausemenu.render(&game.player.hotbar);
		}

		Window::update();
	}

	game.destroy();
	Window::close();

	return 0;
}