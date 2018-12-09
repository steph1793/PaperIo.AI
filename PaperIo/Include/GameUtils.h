#pragma once
#include "SDL.h"
#include <vector>
#include <algorithm>
using namespace std;

bool checkCollision(SDL_Rect *a, SDL_Rect *b);
int filledPolygonRGBA(SDL_Renderer * renderer, const Sint16 * vx, const Sint16 * vy, int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a, SDL_Rect* camera);


bool is_inside_polygon(int x, int y, const Sint16 * vx, const Sint16 * vy, int n);
int* index_extend_polygon(vector<Sint16> vecx, vector<Sint16>vecy, vector<int>to_add_x, vector<int> to_add_y);
int polygon_direction(vector<int> vecx, vector<int> vecy);
void update_polygon(vector<Sint16> &vecx, vector<Sint16> &vecy, vector<int> new_pts_x, vector<int> new_pts_y);
