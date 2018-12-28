#pragma once

#include <SDL.h>
#include "client.h"
#include "MainPlayer.h"
#include <map>

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

extern const int LEVEL_WIDTH;
extern const int LEVEL_HEIGHT;



extern const char* windowTitle;

extern const int SCREEN_FPS;
extern const int SCREEN_TICKS_PER_FRAME;

extern SDL_Window* window;
extern SDL_Surface* screenSurface;
extern SDL_Renderer* Renderer;
extern TTF_Font *gFont;

extern Client *client;
extern bool client_set;
extern bool start;


extern MainPlayer *dot;
extern map<int, Player*> Dots;
extern vector<Rect*> rects_trail;

extern SDL_Rect rects[4]; 