// Server.cpp : Définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include "Utils.h"





int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
		return 0;
	}
	if (!Init_server()) {
		cout << "Initilization unsuccessful\n";
		return 0;
	}
	//Load media
	if (!loadMedia())
	{
		printf("Failed to load media!\n");
		return 0;
	}
	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;

	//While application is running
	while (!quit)
	{
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			gButton.handleEvent(&e, BUTTON_WIDTH, BUTTON_HEIGHT);
			
		}
		if (Start) {
			Send_to_all("start"); Start = false;
		}
		loop();

		//Clear screen
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);

		//Render buttons
		gButton.render(gRenderer);
				

		//Update screen
		SDL_RenderPresent(gRenderer);
			
		
	}

	//Free resources and close SDL
	close();

	return 0;
}