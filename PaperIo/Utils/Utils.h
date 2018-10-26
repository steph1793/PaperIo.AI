#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <sstream>

#include "LTimer.h"

#include "client.h"
#define _SDL_TTF_H


using namespace std;

extern const int SCREEN_WIDTH ;
extern const int SCREEN_HEIGHT ;
extern const char* windowTitle ;

extern const int SCREEN_FPS ;
extern const int SCREEN_TICKS_PER_FRAME;

extern SDL_Window* window ;
extern SDL_Surface* screenSurface ;
extern SDL_Renderer* Renderer;
extern TTF_Font *gFont;

extern Client *client;
extern bool client_set;
extern bool start;
