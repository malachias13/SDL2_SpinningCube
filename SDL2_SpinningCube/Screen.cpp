#include "Screen.h"
#include <iostream>

Screen::Screen()
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(width, hegiht, 0, &window, &renderer);
}

void Screen::pixel(float x, float y)
{
	SDL_FPoint p;
	p.x = x;
	p.y = y;
	points.push_back(p);
}

void Screen::show()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	for (auto& point : points)
	{
		SDL_RenderDrawPointF(renderer, point.x, point.y);
	}

	SDL_RenderPresent(renderer);
}

void Screen::inputHandle()
{
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
		{
			SDL_Quit();
			exit(0);
		}
		if (e.type == SDL_KEYDOWN)
		{
			switch (e.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				std::cout << "Close window\n";
				SDL_Quit();
				exit(0);
				break;
			}
		}
	}

}

void Screen::clear()
{
	points.clear();
}
