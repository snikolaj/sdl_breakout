#ifndef Game_hpp
#define Game_hpp


#include <SDL.h>
#undef main
#include <iostream>
#include <chrono>
#include <cstdint>

// rectangle collision
bool rectColl(SDL_Rect &rect1, SDL_Rect &rect2);
bool paddleWallColl(SDL_Rect& paddle, int& winW, bool direction);
bool ballWallColl(SDL_Rect& ball, std::pair<int, int>& ballVec, int& winW, int& winH);


class Game {
public:
	Game();
	~Game();

	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	void handleEvents();
	void update();
	void render();
	void clean();

	bool running() { return isRunning; };

private:
	bool isRunning;
	SDL_Window* window;
	SDL_Renderer* renderer;

	int winW, winH;

	SDL_Rect paddle;
	SDL_Rect blocks[5][5];
	
	SDL_Rect ball;
	std::pair<int, int> ballVec;
};

#endif
