#pragma once
#include <SDL.h>
#include <vector>
#include "../GameUtils.h"

class Rect {
public:
	Rect(int playerId, SDL_Rect b, int type);
	Rect();
	SDL_Rect box;
	int playerId;
	int RectType;


	void render(SDL_Rect *camera, SDL_Renderer *renderer);
	void updateRect(SDL_Rect, int);
	static void renderRects(std::vector<Rect*>, SDL_Rect *camera, SDL_Renderer *renderer);

};