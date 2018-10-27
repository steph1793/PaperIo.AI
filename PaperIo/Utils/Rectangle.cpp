#include "Rectangle.h"

Rect::Rect(int playerid, SDL_Rect b, int type)
{
	playerId = playerid;
	box = b;
	RectType = type;
}
Rect::Rect()
{
	playerId = 0;
	box = { 0,0,0,0 };
	RectType = 0;
}

void Rect::render(SDL_Rect *camera, SDL_Renderer *renderer)
{
	if (checkCollision(camera, &box)) {
		SDL_Rect fill_rect = { box.x - camera->x, box.y - camera->y, box.w, box.h };
		SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 100);
		SDL_RenderFillRect(renderer, &fill_rect);

	}
}

void Rect::updateRect(SDL_Rect p, int direction)
{
	if (direction ==0) {//horizontal
		if (box.x > p.x) {
			box.w += (-p.x + box.x);
			box.x = p.x;
		}
		else
		{
			box.w = (p.x - box.x);
		}

	}
	if (direction == 1) //vertical
	{
		if (box.y > p.y) {
			box.h += (box.y - p.y);
			box.y = p.y;
		}
		else {
			box.h = p.y - box.y;
		}
	}
}

void Rect::renderRects(std::vector<Rect*> rects, SDL_Rect *camera, SDL_Renderer *renderer) {
	for (auto it = rects.begin(); it != rects.end(); it++)
	{
		(*it)->render(camera, renderer);
	}
}

