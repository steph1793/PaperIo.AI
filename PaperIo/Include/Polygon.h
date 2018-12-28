#pragma once

#include <SDL.h>
#include <vector>
#include <GameUtils.h>

class Polygon {
public:
	Polygon(int playerId, int color, SDL_Rect init);
	Polygon();
	vector<Sint16> vecx;
	vector<Sint16> vecy;
	int playerId;
	int color;

	void render(SDL_Rect *camera, SDL_Renderer *renderer);
	void updatePolygon(SDL_Rect, int);
	
};