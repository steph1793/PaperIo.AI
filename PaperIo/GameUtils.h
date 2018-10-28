#pragma once
#include "SDL.h"

bool checkCollision(SDL_Rect *a, SDL_Rect *b);

int filledPolygonRGBA(SDL_Renderer * renderer, const Sint16 * vx, const Sint16 * vy, int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a, SDL_Rect* camera);