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



int main(int argc, char** argv)
{
	if(!Init()) return 0; // à redefinir (par exemple boucle while dans le init pour essayer de rconnecter le joueur encore et encore jusqu'à ce que le serveur soit allumé).
	App();
	Clean();
	return 0;
} 