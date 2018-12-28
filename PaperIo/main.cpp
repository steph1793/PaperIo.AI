#include <Utils.h>
using namespace std;


void App() {

	 SDL_Event e;
	 bool quit = false;

	 SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	SDL_Color textColor = { 0, 0, 0, 255 };
	LTimer fpsTimer;
	LTimer capTimer;

	//In memory text stream
	std::stringstream timeText;
	//Start counting frames per second
	int countedFrames = 0;
	fpsTimer.start();

	while (!quit) {
		//Start cap timer
		capTimer.start();

		if (!start)
			before_game_loop();
		if (start)
			load_dots();
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			if (start)
				dot->handleEvent(e);
		}

		if(start) {

			update_dots_moves();
			move_dots(Renderer);
			dot->SetCamera(&camera);
			if (dot->dead) break;
		}
	
		//Clear screen
		SDL_SetRenderDrawColor(Renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(Renderer);
		SDL_SetRenderDrawColor(Renderer, 0xF4, 0x0D, 0x42, 0xFF);
		rects[0] = { 0 - camera.x,100 - camera.y ,LEVEL_WIDTH,10 }; rects[1] = { LEVEL_WIDTH - 110 - camera.x,100 - camera.y,10,LEVEL_HEIGHT }; rects[2] = { 100 - camera.x,LEVEL_HEIGHT - 110 - camera.y,LEVEL_WIDTH,10 }; rects[3] = { 100 - camera.x,0 - camera.y,10,LEVEL_HEIGHT };
		SDL_RenderFillRects(Renderer, rects, 4);
		render_polygons(&camera, Renderer);
		SDL_SetRenderDrawBlendMode(Renderer, SDL_BLENDMODE_BLEND);
		Rect::renderRects(rects_trail, &camera, Renderer);
		render_dots(&camera, Renderer);
		//Update screen
		SDL_RenderPresent(Renderer);

		//If frame finished early
		int frameTicks = capTimer.getTicks();
		if (frameTicks < SCREEN_TICKS_PER_FRAME)
		{
			//Wait remaining time
			SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
		}

	}



	
}

int polygonArea()
{
	int numPoints = 4;
	const Sint16 X[4] = { 10,60,60,10 };
	const Sint16 Y[4] = { 10,10,60,60 };
	int area = 0;         // Accumulates area in the loop
	int j = numPoints - 1;  // The last vertex is the 'previous' one to the first

	for (int i = 0; i < numPoints; i++)
	{
		area = area + (X[j] + X[i]) * (Y[j] - Y[i]);
		j = i;  //j is previous vertex to i
	}
	return abs(area) / 2;
}



int main(int argc, char** argv)
{

	


	//std::vector<Sint16> vecx(vx, vx + 4);
	//std::vector<Sint16> vecy(vy, vy + 4);
	//int aire = polygonArea();
	/*
	Sint16 vx[18] = { 1, 10, 10, 40, 40, 70, 70, 100, 100, 120, 120, 70, 70, 40, 40, 10, 10, 1 };
	Sint16 vy[18] = { 30, 30, 10, 10, 30, 30, 0, 0, 50, 50, 80, 80, 100, 100, 120, 120, 60, 60};
	

	//std::vector<int> vecx(vx, vx + 18);
	//std::vector<int> vecy(vy, vy + 18);
	//cout << polygon_direction(vecx, vecy);
	//int result = is_inside_polygon(120,30, vx, vy, 18);
	std::vector<int> vecx(vx, vx + 18);
	std::vector<int> vecy(vy, vy + 18);

	std::vector<int> add_x; 
	add_x.push_back(5);
	add_x.push_back(5); 
	add_x.push_back(110);
	add_x.push_back(110);

	std::vector<int> add_y;
	add_y.push_back(30);
	add_y.push_back(0);
	add_y.push_back(0);
	add_y.push_back(50);

	//update_polygon(vecx, vecy, add_x, add_y);

	//int * res = index_extend_polygon(vecx, vecy, add_x, add_y);
	//if(res) cout << res[0] << endl << res[1] << endl;
	//else cout << "Nothing" << endl;
	*/
	if(!Init()) return 0; // à redefinir (par exemple boucle while dans le init pour essayer de rconnecter le joueur encore et encore jusqu'à ce que le serveur soit allumé).
	App();
	Clean();
	return 0;
} 