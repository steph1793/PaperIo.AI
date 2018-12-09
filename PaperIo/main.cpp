#include <Utils.h>
using namespace std;


void App() {
	Sint16 vx[18] = { 1, 10, 10, 40, 40, 70, 70, 100, 100, 120, 120, 70, 70, 40, 40, 10, 10, 1 };
	Sint16 vy[18] = { 30, 30, 10, 10, 30, 30, 0, 0, 50, 50, 80, 80, 100, 100, 120, 120, 60, 60};

	 SDL_Event e;
	 bool quit = false;

	 SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	// LTexture gBGTexture;
	 //if (!gBGTexture.loadFromFile("Media/Images/bg.png", Renderer))
	 //{
		// printf("Failed to load background texture!\n");
	 //}

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
		}
	
		//Clear screen
		SDL_SetRenderDrawColor(Renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(Renderer);
		//gBGTexture.render(0, 0, Renderer, &camera);
		//Render textures
		//gFPSTextTexture.render((SCREEN_WIDTH - gFPSTextTexture.getWidth()) / 2, (SCREEN_HEIGHT - gFPSTextTexture.getHeight()) / 2);
;		

		int r = filledPolygonRGBA(Renderer, vx, vy, 18, 255, 0, 255, 255, &camera);
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



int main(int argc, char** argv)
{
	//Sint16 vx[18] = { 1, 10, 10, 40, 40, 70, 70, 100, 100, 120, 120, 70, 70, 40, 40, 10, 10, 1 };
	//Sint16 vy[18] = { 30, 30, 10, 10, 30, 30, 0, 0, 50, 50, 80, 80, 100, 100, 120, 120, 60, 60};
	//int result = is_inside_polygon(120,30, vx, vy, 18);
	//cout << result << endl;
	if(!Init()) return 0; // à redefinir (par exemple boucle while dans le init pour essayer de rconnecter le joueur encore et encore jusqu'à ce que le serveur soit allumé).
	App();
	Clean();
	return 0;
} 