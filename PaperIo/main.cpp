#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Utils.h"
#include "Player\Dot.h"

using namespace std;
TTF_Font *loadFont(string font_path, int font_size)
{
	//Open the font
	TTF_Font *gFont = TTF_OpenFont(font_path.c_str(), font_size);
	if (gFont == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
	}

	return gFont;
}


SDL_Texture* loadTexture(std::string path)
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(Renderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}
bool Init() {


	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "SDL could not initialize!!!\n";
		return false;
	}
	window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!window) {
		cout << "Window could not initialize !!!\n";
		return false;
	}

	Renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (Renderer == NULL)
	{
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	SDL_SetRenderDrawColor(Renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return false;
	}
	if (TTF_Init() == -1)
	{
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		return false;
	}
	 //gFont = loadFont("Media/Fonts/lazy.ttf", 28);

	return true;
}
void Clean() {
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void App() {

	 SDL_Event e;
	 bool quit = false;

	 Dot dot;


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
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			dot.handleEvent(e);
		}

		dot.move();

		//Clear screen
		SDL_SetRenderDrawColor(Renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(Renderer);

		//Render textures
		//gFPSTextTexture.render((SCREEN_WIDTH - gFPSTextTexture.getWidth()) / 2, (SCREEN_HEIGHT - gFPSTextTexture.getHeight()) / 2);

		dot.render();

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

	Init();
	App();
	Clean();
	return 0;
}