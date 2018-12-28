#pragma once
#include "SDL.h"
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

extern  SDL_Rect colors[8];

#include "clipper.hpp"

//from clipper.hpp ...
//typedef signed long long cInt;
//struct IntPoint {cInt X; cInt Y;};
//typedef std::vector<IntPoint> Path;
//typedef std::vector<Path> Paths;
using namespace ClipperLib;



bool checkCollision(SDL_Rect *a, SDL_Rect *b);
int filledPolygonRGBA(SDL_Renderer * renderer, const Sint16 * vx, const Sint16 * vy, int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a, SDL_Rect* camera);


bool is_inside_polygon(int x, int y,  Sint16 * vx,  Sint16 * vy, int n);
int* index_extend_polygon(vector<Sint16> vecx, vector<Sint16>vecy, vector<int>to_add_x, vector<int> to_add_y);
int polygon_direction(vector<int> vecx, vector<int> vecy);
bool update_polygon(vector<Sint16> &vecx, vector<Sint16> &vecy, vector<int> new_pts_x, vector<int> new_pts_y);

class Point
{
public:
	int x;
	int y;
	bool in = true;
	Point(){}
	Point(int X, int Y) { x = X; y = Y; }
	Point(int X, int Y, bool In) { x = X; y = Y; in = In; }
};

Point *find_in_out(Point* in, Point *out, Sint16 *vx, Sint16 *vy, int taille);

double aire_polygon(vector<Sint16> &vecx, vector<Sint16> &vecy);
bool update_polygon_adverse(vector<Sint16> &vecx, vector<Sint16> &vecy, vector<Sint16> &new_pts_x, vector<Sint16> &new_pts_y);

