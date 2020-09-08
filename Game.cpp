#include "Game.h"

Game::Game() {
	ballVec.first = 5;
	ballVec.second = -5;
}

Game::~Game() {

}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
	winW = width;
	winH = height;

	paddle.w = width / 5;
	paddle.h = height / 40;
	paddle.x = width / 2 - paddle.w / 2;
	paddle.y = height / 5 * 4;

	ball.w = width / 100;
	ball.h = width / 100;
	ball.x = width / 2 - ball.w / 2;
	ball.y = height / 2 - ball.w / 2;

	int blockW = width / 5;
	int blockH = (height / 4) / 5;

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			blocks[i][j].x = i * blockW;
			blocks[i][j].y = j * blockH;
			blocks[i][j].w = blockW;
			blocks[i][j].h = blockH;
		}
	}

	int flags = 0;
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}
	
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "SDL Initialized" << std::endl;

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window) {
			std::cout << "Window Created" << std::endl;
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			std::cout << "Renderer Created" << std::endl;
		}

		isRunning = true;
	}
	else {
		isRunning = false;
	}
}

void Game::handleEvents() {
	SDL_Event event;
	SDL_PollEvent(&event);

	const Uint8* state = SDL_GetKeyboardState(NULL);

	if (state[SDL_SCANCODE_LEFT]) {
		if (!paddleWallColl(paddle, winW, false)) {
			paddle.x -= winW / 80;
		}
	}
	if (state[SDL_SCANCODE_RIGHT]) {
		if (!paddleWallColl(paddle, winW, true)) {
			paddle.x += winW / 80;
		}
	}


	switch (event.type) {
	case SDL_QUIT:
		isRunning = false;
		break;

	default:
		break;
	}

}

void Game::update() {
	if (rectColl(paddle, ball)) {
		ballVec.second *= -1;
	}
	if (ballWallColl(ball, ballVec, winW, winH)) {
		ball.x = winW / 2;
		ball.y = winH / 2;
	}
	
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (rectColl(ball, blocks[i][j])) {
				blocks[i][j].x = 0;
				blocks[i][j].y = 0;
				blocks[i][j].w = 0;
				blocks[i][j].h = 0;
				ballVec.second *= -1;
			}
		}
	}

	ball.x += ballVec.first;
	ball.y += ballVec.second;
}

void Game::render() {
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	
	SDL_RenderFillRect(renderer, &paddle);
	SDL_RenderFillRect(renderer, &ball);
	SDL_RenderDrawRects(renderer, *blocks, 25);
	
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderPresent(renderer);
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game Cleaned";
}



bool rectColl(SDL_Rect& rect1, SDL_Rect& rect2) {
	if (rect1.x < rect2.x + rect2.w &&
		rect1.x + rect1.w > rect2.x &&
		rect1.y < rect2.y + rect2.h &&
		rect1.y + rect1.h > rect2.y) {
		return true;
	}
	return false;
}

// true = right, false = left
bool paddleWallColl(SDL_Rect& paddle, int& winW, bool direction) { 
	if (paddle.x < 0 && !direction) {
		return true;
	}
	if (paddle.x + paddle.w > winW && direction) {
		return true;
	}
	return false;
}

// true = bottom, false = wall
bool ballWallColl(SDL_Rect& ball, std::pair<int, int>& ballVec, int& winW, int& winH) { 
	if (ball.x + ballVec.first < 0 || ball.x + ballVec.first > winW) {
		ballVec.first *= -1;
		return false;
	}
	if (ball.y + ballVec.second < 0) {
		ballVec.second *= -1;
		return false;
	}
	if (ball.y + ballVec.second > winH) {
		return true;
	}
	return false;
}
