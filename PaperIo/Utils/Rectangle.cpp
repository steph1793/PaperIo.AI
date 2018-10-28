#include "Rectangle.h"

SDL_Rect colors[8]{
	{204,0,204,255},
	{204,0,102,255},
	{102,0,204,255},
	{0,0,204,255},
	{0,153,76,255},
	{153,153,0,255},
	{255,255,51,255},
	{255,153,51,255}
};

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
	int alpha = 0;
	if (checkCollision(camera, &box)) {
		SDL_Rect fill_rect = { box.x - camera->x, box.y - camera->y, box.w, box.h };
		if (RectType == 0) alpha = 100;
		else alpha = 255;
		SDL_SetRenderDrawColor(renderer, colors[color].x, colors[color].y, colors[color].w, alpha);
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

