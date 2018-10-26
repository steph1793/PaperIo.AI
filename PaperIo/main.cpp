#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Utils/Utils.h"
#include "Player/Player.h"

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


	client = new Client("192.168.43.176");
	if (!client->Init()) { printf("Client initialization failed.\n"); return false; }
	else  client_set = true;

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

void load_dots()
{
	if (!dot->gDotTexture.loadFromFile("Media/Images/dot.bmp", Renderer))
	{
		printf("failed loading Main dot!!!\n");
	}

	for (auto i = Dots.begin(); i != Dots.end(); i++)
	{
		if (!i->second->gDotTexture.loadFromFile("Media/Images/dot.bmp", Renderer))
		{
			printf("failed loading dot %d !!!\n", i->first);
		}
	}
}

void render_dots() {
	dot->render();
	for (auto i = Dots.begin(); i != Dots.end(); i++)
	{
		i->second->render();
	}
}

void move_dots() {
	dot->move();
	for (auto i = Dots.begin(); i != Dots.end(); i++)
	{
		i->second->move();
	}
}


void update_dots_moves() {
	string buff = client->GetResponse();
	if (buff.length() > 3) {
		char b[100];
		strcpy_s(b, buff.c_str());
		char *next = NULL;
		char *s = strtok_s(b, " ",&next);
		char *s2 = strtok_s(NULL, " ",&next);
		int id = atoi(s);
		Player *tmp = NULL;
		if (id == client->id) { ///////il s'agit du main
			tmp = dot;
		}
		else
			tmp = Dots.find(id)->second;
		if (strcmp(s2, "UP") == 0) {
			tmp->mVelX = 0;
			tmp->mVelY = - tmp->DOT_VEL;
		}
		if (strcmp(s2, "DOWN") == 0) {
			tmp->mVelX = 0;
			tmp->mVelY = tmp->DOT_VEL;
		}
		if (strcmp(s2, "RIGHT") == 0) {
			tmp->mVelX = tmp->DOT_VEL;
			tmp->mVelY = 0;
		}
		if (strcmp(s2, "LEFT") == 0) {
			tmp->mVelX = -tmp->DOT_VEL;
			tmp->mVelY = 0;
		}
	}
}


void before_game_loop() {
	string buff = client->GetResponse();
	char  tmp[1000] ;
	strcpy_s(tmp, buff.c_str());
	if (strlen(tmp)>3) {

		if (buff == "start") {
			start = true;
		}
		else {
			char *next_token = NULL;
			char *next_token2 = NULL;

			char *s = strtok_s(tmp, ";", &next_token);

			while (s) {
				char* s1 = strtok_s(s, " ", &next_token2);
				char* s2 = strtok_s(NULL, " ", &next_token2);
				char* s3 = strtok_s(NULL, " ", &next_token2);
				char* s4 = strtok_s(NULL, " ", &next_token2);
				if (strcmp(s2, "connected") == 0) {
					if (!dot->set) {
						dot->id = atoi(s1);
						dot->mPosX = atoi(s3);
						dot->mPosY = atoi(s4);
						dot->set = true;
						client->id = atoi(s1);
						cout << "dot created" << endl;
					}
					else {
						Dots.insert(pair<int, Player*>(atoi(s1), new Player(atoi(s1), atoi(s3), atoi(s4))));
					}
				}

				s = strtok_s(NULL, ";", &next_token);
			}
		}
		
	}
	
}

void App() {


	 SDL_Event e;
	 bool quit = false;



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
			move_dots();

		}
	
		//Clear screen
		SDL_SetRenderDrawColor(Renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(Renderer);

		//Render textures
		//gFPSTextTexture.render((SCREEN_WIDTH - gFPSTextTexture.getWidth()) / 2, (SCREEN_HEIGHT - gFPSTextTexture.getHeight()) / 2);

		render_dots();
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