#include "Game.h"

using namespace std;
using namespace chrono;

using frame = duration<int32_t, ratio<1, 60>>;
using ms = duration<float, milli>;

Game* game = nullptr;

int main() {
	game = new Game();
	game->init("Breakout", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false);

	time_point<steady_clock> fpsTimer(steady_clock::now());
	frame FPS{};

	while (game->running()) {
		FPS = duration_cast<frame>(steady_clock::now() - fpsTimer);

		if (FPS.count() >= 1) {
			fpsTimer = steady_clock::now();
			//cout << "LastFrame: " << duration_cast<ms>(FPS).count() << "ms  |  FPS: " << FPS.count() * 60 << endl;
			game->handleEvents();
			game->update();
			game->render();
		}
		
	}

	game->clean();
	return 0;
}
