#pragma once

#include <SDL2/SDL.h>
#include <vector>

class Screen
{
	SDL_Event e;
	SDL_Window* window;
	SDL_Renderer* renderer;
	std::vector<SDL_FPoint> points;

	int width = 1080;
	int hegiht = 720;

public:
	Screen();

	void pixel(float x, float y);
	void show();
	void inputHandle();
	void clear();

public:
	int getWidth() { return width; }
	int getHeight() { return hegiht; }
};

