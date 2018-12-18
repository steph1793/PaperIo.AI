#include <Polygon.h>

Polygon::Polygon(int player_id, int color, SDL_Rect init) {
	playerId = player_id;
	this->color = color;
	vecx.push_back(init.x);
	vecx.push_back(init.x + init.w);
	vecx.push_back(init.x + init.w);
	vecx.push_back(init.x);

	vecy.push_back(init.y);
	vecy.push_back(init.y);
	vecy.push_back(init.y + init.h);
	vecy.push_back(init.y + init.h);
}
Polygon::Polygon(){}

void Polygon::render(SDL_Rect *camera, SDL_Renderer *renderer) {
	Sint16 *vx = &vecx[0];
	Sint16 *vy = &vecy[0];
	int size = vecx.size();
	filledPolygonRGBA(renderer, vx, vy, size, colors[color].x , colors[color].y, colors[color].w,255, camera);
}


void Polygon::updatePolygon(SDL_Rect, int) {

}

