#include <GameUtils.h>
#include <math.h>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>>



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



bool checkCollision(SDL_Rect *a, SDL_Rect* b)
{
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A
	leftA = a->x;
	rightA = a->x + a->w;
	topA = a->y;
	bottomA = a->y + a->h;

	//Calculate the sides of rect B
	leftB = b->x;
	rightB = b->x + b->w;
	topB = b->y;
	bottomB = b->y + b->h;

	//If any of the sides from A are outside of B
	if (bottomA <= topB)
	{
		return false;
	}

	if (topA >= bottomB)
	{
		return false;
	}

	if (rightA <= leftB)
	{
		return false;
	}

	if (leftA >= rightB)
	{
		return false;
	}

	//If none of the sides from A are outside B
	return true;
}


static int *gfxPrimitivesPolyIntsGlobal = NULL;

static int gfxPrimitivesPolyAllocatedGlobal = 0;


int _gfxPrimitivesCompareInt(const void *a, const void *b)
{
	return (*(const int *)a) - (*(const int *)b);
}


int hline(SDL_Renderer * renderer, Sint16 x1, Sint16 x2, Sint16 y, SDL_Rect *camera)
{
	SDL_Rect r = SDL_Rect();
	r.x = (int)x1;
	r.y = y;
	r.w = abs(x1 - x2);
	r.h = 1;
	return SDL_RenderDrawLine(renderer, x1 -camera->x, y-camera->y, x2 -camera->x, y-camera->y);;
	
}
int filledPolygonRGBAMT(SDL_Renderer * renderer, const Sint16 * vx, const Sint16 * vy, int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a, int **polyInts, int *polyAllocated, SDL_Rect *camera)
{
	int result;
	int i;
	int y, xa, xb;
	int miny, maxy;
	int x1, y1;
	int x2, y2;
	int ind1, ind2;
	int ints;
	int *gfxPrimitivesPolyInts = NULL;
	int *gfxPrimitivesPolyIntsNew = NULL;
	int gfxPrimitivesPolyAllocated = 0;

	/*
	* Vertex array NULL check
	*/
	if (vx == NULL) {
		return (-1);
	}
	if (vy == NULL) {
		return (-1);
	}

	/*
	* Sanity check number of edges
	*/
	if (n < 3) {
		return -1;
	}

	/*
	* Map polygon cache
	*/
	if ((polyInts == NULL) || (polyAllocated == NULL)) {
		/* Use global cache */
		gfxPrimitivesPolyInts = gfxPrimitivesPolyIntsGlobal;
		gfxPrimitivesPolyAllocated = gfxPrimitivesPolyAllocatedGlobal;
	}
	else {
		/* Use local cache */
		gfxPrimitivesPolyInts = *polyInts;
		gfxPrimitivesPolyAllocated = *polyAllocated;
	}

	/*
	* Allocate temp array, only grow array
	*/
	if (!gfxPrimitivesPolyAllocated) {
		gfxPrimitivesPolyInts = (int *)malloc(sizeof(int) * n);
		gfxPrimitivesPolyAllocated = n;
	}
	else {
		if (gfxPrimitivesPolyAllocated < n) {
			gfxPrimitivesPolyIntsNew = (int *)realloc(gfxPrimitivesPolyInts, sizeof(int) * n);
			if (!gfxPrimitivesPolyIntsNew) {
				if (!gfxPrimitivesPolyInts) {
					free(gfxPrimitivesPolyInts);
					gfxPrimitivesPolyInts = NULL;
				}
				gfxPrimitivesPolyAllocated = 0;
			}
			else {
				gfxPrimitivesPolyInts = gfxPrimitivesPolyIntsNew;
				gfxPrimitivesPolyAllocated = n;
			}
		}
	}

	/*
	* Check temp array
	*/
	if (gfxPrimitivesPolyInts == NULL) {
		gfxPrimitivesPolyAllocated = 0;
	}

	/*
	* Update cache variables
	*/
	if ((polyInts == NULL) || (polyAllocated == NULL)) {
		gfxPrimitivesPolyIntsGlobal = gfxPrimitivesPolyInts;
		gfxPrimitivesPolyAllocatedGlobal = gfxPrimitivesPolyAllocated;
	}
	else {
		*polyInts = gfxPrimitivesPolyInts;
		*polyAllocated = gfxPrimitivesPolyAllocated;
	}

	/*
	* Check temp array again
	*/
	if (gfxPrimitivesPolyInts == NULL) {
		return(-1);
	}

	/*
	* Determine Y maxima
	*/
	miny = vy[0];
	maxy = vy[0];
	for (i = 1; (i < n); i++) {
		if (vy[i] < miny) {
			miny = vy[i];
		}
		else if (vy[i] > maxy) {
			maxy = vy[i];
		}
	}

	/*
	* Draw, scanning y
	*/
	result = 0;
	for (y = miny; (y <= maxy); y++) {
		ints = 0;
		for (i = 0; (i < n); i++) {
			if (!i) {
				ind1 = n - 1;
				ind2 = 0;
			}
			else {
				ind1 = i - 1;
				ind2 = i;
			}
			y1 = vy[ind1];
			y2 = vy[ind2];
			if (y1 < y2) {
				x1 = vx[ind1];
				x2 = vx[ind2];
			}
			else if (y1 > y2) {
				y2 = vy[ind1];
				y1 = vy[ind2];
				x2 = vx[ind1];
				x1 = vx[ind2];
			}
			else {
				continue;
			}
			if (((y >= y1) && (y < y2)) || ((y == maxy) && (y > y1) && (y <= y2))) {
				gfxPrimitivesPolyInts[ints++] = ((65536 * (y - y1)) / (y2 - y1)) * (x2 - x1) + (65536 * x1);
			}
		}

		qsort(gfxPrimitivesPolyInts, ints, sizeof(int), _gfxPrimitivesCompareInt);

		/*
		* Set color
		*/
		result = 0;
		result |= SDL_SetRenderDrawBlendMode(renderer, (a == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
		result |= SDL_SetRenderDrawColor(renderer, r, g, b, a);

		for (i = 0; (i < ints); i += 2) {
			xa = gfxPrimitivesPolyInts[i] + 1;
			xa = (xa >> 16) + ((xa & 32768) >> 15);
			xb = gfxPrimitivesPolyInts[i + 1] - 1;
			xb = (xb >> 16) + ((xb & 32768) >> 15);
			result |= hline(renderer, xa, xb, y, camera);
		}
	}

	return (result);
}

/*!
\brief Draw filled polygon with alpha blending.

\param renderer The renderer to draw on.
\param vx Vertex array containing X coordinates of the points of the filled polygon.
\param vy Vertex array containing Y coordinates of the points of the filled polygon.
\param n Number of points in the vertex array. Minimum number is 3.
\param color The color value of the filled polygon to draw (0xRRGGBBAA).

\returns Returns 0 on success, -1 on failure.
*/
int filledPolygonColor(SDL_Renderer * renderer, const Sint16 * vx, const Sint16 * vy, int n, Uint32 color, SDL_Rect *camera)
{
	Uint8 *c = (Uint8 *)&color;
	return filledPolygonRGBAMT(renderer, vx, vy, n, c[0], c[1], c[2], c[3], NULL, NULL, camera);
}

/*!
\brief Draw filled polygon with alpha blending.

\param renderer The renderer to draw on.
\param vx Vertex array containing X coordinates of the points of the filled polygon.
\param vy Vertex array containing Y coordinates of the points of the filled polygon.
\param n Number of points in the vertex array. Minimum number is 3.
\param r The red value of the filled polygon to draw.
\param g The green value of the filled polygon to draw.
\param b The blue value of the filed polygon to draw.
\param a The alpha value of the filled polygon to draw.

\returns Returns 0 on success, -1 on failure.
*/
int filledPolygonRGBA(SDL_Renderer * renderer, const Sint16 * vx, const Sint16 * vy, int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a, SDL_Rect *camera)
{
	return filledPolygonRGBAMT(renderer, vx, vy, n, r, g, b, a, NULL, NULL, camera);
}


int direction(int x1, int x2) {
	if (x1 < x2) {
		return 1;
	}
	else if (x1 == x2) return 0;
	else return -1;
}


bool is_concave(int x1, int x2, int x3, int y1, int y2, int y3) {
	bool result = false;
	if (y1 == y2) {
		if (y3 < y1) result = true;
	}
	else if (y1 == y3) {
		if (y2 < y1) result = true;
	}
	if (direction(x2, x3) < 0) result = !result;
	return result;
}

bool is_inside_polygon(int x, int y,  Sint16 * vx,  Sint16 * vy, int n) {
	int compteur_left = 0;
	int compteur_right = 0;
	int pre, post = 0;
	vector<int> vec;
	for (int i = 0; i < n; i++) {
		if (std::find(vec.begin(), vec.end(), vx[i]) != vec.end()) {
			continue;
		}
		post = (i + 1 >= n) ? 0 : i+1;
		pre = (i - 1 < 0) ? n - 1 : i-1;
		if (y == vy[i]) {
			if (x > vx[i]) {
				if (!is_concave(vx[i], vx[pre], vx[post], vy[i], vy[pre], vy[post]))
				{
					compteur_left++;
					vec.push_back(vx[i]);
				}
			}
			else if (x < vx[i]) {
				if (!is_concave(vx[i], vx[pre], vx[post], vy[i], vy[pre], vy[post]))
				{
					compteur_right++;
					vec.push_back(vx[i]);
				}
			}
			else return true;
		}
		else {
			for (int j = i + 1; j < n; j++) {
				bool added = false;

				if (vx[i] == vx[j]) {
					if ((y <= vy[j] && y >= vy[i]) || (y >= vy[j] && y <= vy[i])) {
						if (x < vx[i]) { compteur_right++; vec.push_back(vx[i]); added = true; }
						else if (x > vx[i]) { compteur_left++; vec.push_back(vx[i]); added = true; }
						else return true;
					}
					if (!added && j == i+1) {
						i++; // we skip the next point too
					}
					break;
				}
			}
		}
	}
	if ( (compteur_left%2==1 && compteur_right%2==1) ||
		(compteur_left*compteur_right !=0 && (compteur_left+compteur_right)%2==1)) return true;
	else return false;
}

int* index_extend_polygon(vector<Sint16> vecx, vector<Sint16>vecy, vector<int>to_add_x, vector<int> to_add_y) {

	int out_x = *(to_add_x.begin());
	int out_y = *(to_add_y.begin());
	int in_x = *(to_add_x.end()-1);
	int in_y = *(to_add_y.end()-1);


	int index_out = -1, index_in = -1, index = 0;
	int x3 = 0, y3 = 0;
	for (auto itx = vecx.begin(), ity = vecy.begin(); itx != vecx.end(); ity++, itx++) {
		//0.5 *[x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)]
		int x1 = *itx;
		int x2 = (itx + 1 != vecx.end()) ? *(itx + 1) : *(vecx.begin());

		int y1 = *ity;
		int y2 = (ity + 1 != vecy.end()) ? *(ity + 1) : *(vecy.begin());

		x3 = out_x;
		y3 = out_y;
		if ((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) == 0) {
			if (y1 == y2) {
				if ((x1 <= x3 && x2 >= x3) || (x1 >= x3 && x2 <= x3)) index_out = index;
			}
			else if (x1 == x2) {
				if ((y1 <= y3 && y2 >= y3) || (y1 >= y3 && y2 <= y3)) index_out = index;
			}
		}
		x3 = in_x;
		y3 = in_y;

		if ((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) == 0) {
			if (y1 == y2) {
				if ((x1 <= x3 && x2 >= x3) || (x1 >= x3 && x2 <= x3)) index_in = index;
			}
			if (x1 == x2) {
				if ((y1 <= y3 && y2 >= y3) || (y1 >= y3 && y2 <= y3)) index_in = index;
			}
		}
		if (index_in >= 0 && index_out >= 0) return new int[2] { index_out, index_in };
		index++;
	}
	return NULL;
}

int polygon_direction(vector<int> vecx, vector<int> vecy) {
	
	int result = 0; int x1, x2, y1, y2 = 0;
	for (auto itx = vecx.begin(), ity = vecy.begin(); itx != vecx.end(); itx++, ity++) {

		x1 = *itx;
		x2 = (itx + 1 != vecx.end()) ? *(itx + 1) : *vecx.begin();
		y1 = *ity;
		y2 = (ity + 1 != vecy.end()) ? *(ity + 1) : *vecy.begin();
		result += (x2 - x1)*(y2 + y1);
	}
	return result;
}

double aire_polygon(vector<Sint16> &vecx, vector<Sint16> &vecy) {

	int aire =0 ;
	int somme = 0;
	auto itx = vecx.begin();
	auto ity = vecy.begin();
	int x1, x2, y1, y2 = 0;
	int i = 0;
	int taille = vecx.size();
	while (true) {
		if (i < taille-1) {
			x1 = (*itx); x2 = *(itx + 1); y1 = *ity;  y2 = *(ity + 1);
			somme = (x1 + x2) * (y1 - y2);
			aire += somme;

		}
		else {
			x1 = *(vecx.end() - 1); x2 = *(vecx.begin()); y2 = *(vecy.begin()); y1 = *(vecy.end() - 1);
			somme = (x1 + x2) * (y1 - y2);
			aire += somme;
			break;
		}
		itx++;
		ity++;
		i++;
	}
	return abs( aire )/ 2.0;
}

bool is_inside_all(vector<Sint16> &vecx, vector<Sint16> &vecy, vector<Sint16> new_pts_x, vector<Sint16> new_pts_y) {
	for (auto itx = vecx.begin(), ity = vecy.begin(); itx != vecx.end(); itx++, ity++) {
		if (!is_inside_polygon(*itx, *ity, &new_pts_x[0], &new_pts_y[0], new_pts_x.size())) {
			return false;
		}
	}
	return true;
}


bool update_polygon(vector<Sint16> &vecx, vector<Sint16> &vecy, vector<int> new_pts_x, vector<int> new_pts_y) {
	vector<Sint16> vecx_temp1(vecx);
	vector<Sint16> vecy_temp1(vecy);

	vector<Sint16> vecx_temp2(vecx);
	vector<Sint16> vecy_temp2(vecy);

	int direct = polygon_direction(new_pts_x, new_pts_y);
	int * indexes = index_extend_polygon(vecx, vecy, new_pts_x, new_pts_y);

	int aire1 = -1;
	int aire2 = -1;



	if (indexes) {
		int out_ind = indexes[0];
		int in_ind = indexes[1];
		int min = out_ind < in_ind ? out_ind : in_ind;
		int max = out_ind > in_ind ? out_ind : in_ind;

		if (new_pts_x.size() == 2) {
			vector<int> x_temp;
			vector<int> y_temp;
			if (min > 0 && max < vecx.size()-1) {
				x_temp.insert(x_temp.begin(), vecx.begin() + (min - 1), vecx.begin() + (max + 1));
				y_temp.insert(y_temp.begin(), vecy.begin() + (min - 1), vecy.begin() + (max + 1));
			}
			else {
				
			}
		}


		//if (out_ind != in_ind) {
		vecx_temp1.erase(vecx_temp1.begin() + (min + 1), ((max == vecx_temp1.size() - 1) ? vecx_temp1.end() : vecx_temp1.begin() + (max + 1)));
		vecy_temp1.erase(vecy_temp1.begin() + (min + 1), ((max == vecy_temp1.size() - 1) ? vecy_temp1.end() : vecy_temp1.begin() + (max + 1)));

		vecx_temp2.erase(vecx_temp2.begin(), ((min == vecx_temp2.size() - 1) ? vecx_temp2.end() : vecx_temp2.begin() + min + 1));
		vecy_temp2.erase(vecy_temp2.begin(), ((min == vecy_temp2.size() - 1) ? vecy_temp2.end() : vecy_temp2.begin() + min + 1));
		if (max - min != vecx_temp2.size()) {
			int t = max - min;
			vecx_temp2.erase(vecx_temp2.begin() + t, vecx_temp2.end());
			vecy_temp2.erase(vecy_temp2.begin() + t, vecy_temp2.end());
		}
		//}
		if (new_pts_x.size() == 2) {
			vector<int> x_temp;
			vector<int> y_temp;
			if (vecx_temp1.size() < vecx_temp2.size()) {
				x_temp.insert(x_temp.begin(), vecx_temp1.begin(), vecx_temp1.end());
				y_temp.insert(y_temp.begin(), vecy_temp1.begin(), vecy_temp1.end());
			}
			else {
				x_temp.insert(x_temp.begin(), vecx_temp2.begin(), vecx_temp2.end());
				y_temp.insert(y_temp.begin(), vecy_temp2.begin(), vecy_temp2.end());
			}
			bool out_res = false;
			if (vecx[out_ind] == *(x_temp.end() - 1) && vecy[out_ind] == *(y_temp.end() - 1)) {
				x_temp.push_back(new_pts_x[0]);
				y_temp.push_back(new_pts_y[0]);
				x_temp.push_back(new_pts_x[1]);
				y_temp.push_back(new_pts_y[1]);
			}
			else if (vecx[in_ind] == *(x_temp.end() - 1) && vecy[in_ind] == *(y_temp.end() - 1)) {
				x_temp.push_back(new_pts_x[1]);
				y_temp.push_back(new_pts_y[1]);
				x_temp.push_back(new_pts_x[0]);
				y_temp.push_back(new_pts_y[0]);
				out_res = true;

			}
			else if (vecx[out_ind] == *(x_temp.begin()) && vecy[out_ind] == *(y_temp.begin() )) {
				x_temp.push_back(new_pts_x[1]);
				y_temp.push_back(new_pts_y[1]);
				x_temp.push_back(new_pts_x[0]);
				y_temp.push_back(new_pts_y[0]);
				out_res = true;

			}
			else if (vecx[in_ind] == *(x_temp.begin()) && vecy[in_ind] == *(y_temp.begin())) {
				x_temp.push_back(new_pts_x[0]);
				y_temp.push_back(new_pts_y[0]);
				x_temp.push_back(new_pts_x[1]);
				y_temp.push_back(new_pts_y[1]);

			}

			direct = polygon_direction(x_temp, y_temp);
			if (out_res) direct = -direct;
		}

		if (direct > 0) {
			reverse(new_pts_x.begin(), new_pts_x.end());
			reverse(new_pts_y.begin(), new_pts_y.end());
		}


		if ((out_ind <= in_ind && direct<0) || (out_ind >= in_ind && direct > 0)) {
			if (out_ind == in_ind ) {
				if(is_inside_all(vecx_temp1, vecy_temp1, vector<Sint16>(new_pts_x.begin(), new_pts_x.end()), 
					vector<Sint16>(new_pts_y.begin(), new_pts_y.end()))) {
					if (min + 1 == vecx_temp1.size()) {
						min = -1;
					}
					if (direct < 0) {
						reverse(vecx_temp1.begin() + (min + 1), vecx_temp1.end());
						reverse(vecy_temp1.begin() + (min + 1), vecy_temp1.end());
					}
					else {
						reverse(vecx_temp1.begin(), vecx_temp1.begin() + (min + 1));
						reverse(vecy_temp1.begin(), vecy_temp1.begin() + (min + 1));
					}
				}
			}

			vecx_temp1.insert(vecx_temp1.begin() + (min + 1), new_pts_x.begin(), new_pts_x.end());/////////
			vecy_temp1.insert(vecy_temp1.begin() + (min + 1), new_pts_y.begin(), new_pts_y.end());

			//if (out_ind != in_ind) {
			reverse(vecx_temp2.begin(), vecx_temp2.end());
			reverse(vecy_temp2.begin(), vecy_temp2.end());
			vecx_temp2.insert(vecx_temp2.begin(), new_pts_x.begin(), new_pts_x.end());
			vecy_temp2.insert(vecy_temp2.begin(), new_pts_y.begin(), new_pts_y.end());
			//}
		}
		else {
			reverse(vecx_temp1.begin(), vecx_temp1.end());
			reverse(vecy_temp1.begin(), vecy_temp1.end());
			vecx_temp1.insert(vecx_temp1.begin() +(min+1), new_pts_x.begin(), new_pts_x.end());///////// si ça deconne enlevé le min + 1 ici
			vecy_temp1.insert(vecy_temp1.begin() + (min+1), new_pts_y.begin(), new_pts_y.end());

			vecx_temp2.insert(vecx_temp2.begin(), new_pts_x.begin(), new_pts_x.end());
			vecy_temp2.insert(vecy_temp2.begin(), new_pts_y.begin(), new_pts_y.end());
		}

		aire1 = aire_polygon(vecx_temp1, vecy_temp1);
		aire2 = aire_polygon(vecx_temp2, vecy_temp2);
	}
	if (aire1 > 0 || aire2 > 0) {
		vecx.clear();
		vecy.clear();
		if (aire1 < aire2) {
			vecx.insert(vecx.begin(), vecx_temp2.begin(), vecx_temp2.end());
			vecy.insert(vecy.begin(), vecy_temp2.begin(), vecy_temp2.end());
		}
		else {
			vecx.insert(vecx.begin(), vecx_temp1.begin(), vecx_temp1.end());
			vecy.insert(vecy.begin(), vecy_temp1.begin(), vecy_temp1.end());
		}
		return true;
	}
	return false;
}


bool update_polygon_adverse(vector<Sint16> &vecx, vector<Sint16> &vecy, vector<Sint16> &adv_vecx, vector<Sint16> &adv_vecy) {

	Paths subj(1), clip(1), solution;
	reverse(vecx.begin(), vecx.end());
	reverse(vecy.begin(), vecy.end());
	for (auto itx = vecx.begin(), ity = vecy.begin(); itx != vecx.end(); itx++, ity++) {
		subj[0] << IntPoint(*itx, *ity);
	}
	for (auto itx = adv_vecx.begin(), ity = adv_vecy.begin(); itx != adv_vecx.end(); itx++, ity++) {
		clip[0] << IntPoint(*itx, *ity);
	}

	//perform intersection ...
	Clipper c;
	c.AddPaths(subj, ptSubject, true);
	c.AddPaths(clip, ptClip, true);
	c.Execute(ctDifference, solution, pftNonZero, pftNonZero);

	vecx.clear();
	vecy.clear();
	for (auto it = solution[0].begin(); it != solution[0].end(); it++) {
		vecx.push_back((*it).X);
		vecy.push_back((*it).Y);
	}

	return true;
}

Point *find_in_out(Point *p2, Point *p1, Sint16 *vx, Sint16 *vy, int taille) {

	if (p1->x == p2->x) {
		for (int i = 1; i < taille; i++) {
			if (vy[i - 1] == vy[i]) {
				if ((vx[i-1] <= p1->x && vx[i] >= p1->x) || (vx[i-1] >= p1->x && vx[i] <= p1->x)) {
					if ((vy[i] <= p1->y && vy[i] >= p2->y) || (vy[i] >= p1->y && vy[i] <= p2->y)) {
						return new Point(p1->x, vy[i]);
					}
				}
			}
		}
	}
	else {//p1->y == p2->y

		for (int i = 1; i < taille; i++) {
			if (vx[i - 1] == vx[i]) {
				if ((vy[i - 1] <= p1->y && vy[i] >= p1->y) || (vy[i - 1] >= p1->y && vy[i] <= p1->y)) {
					if ((vx[i] <= p1->x && vx[i] >= p2->x) || (vx[i] >= p1->x && vx[i] <= p2->x)) {
						return new Point(vx[i], p1->y);
					}
				}
			}
		}
		if (vx[taille-1] == vx[0]) {
			if ((vy[taille - 1] <= p1->y && vy[0] >= p1->y) || (vy[taille - 1] >= p1->y && vy[0] <= p1->y)) {
				if ((vx[0] <= p1->x && vx[0] >= p2->x) || (vx[0] >= p1->x && vx[0] <= p2->x)) {
					return new Point(vx[0], p1->y);
				}
			}
		}
	}

	
	return NULL;
}
