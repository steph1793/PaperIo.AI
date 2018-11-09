#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include "LTimer.h"
#include "Globals.h"
#include "GameUtils.h"

#define _SDL_TTF_H


using namespace std;


bool Init();
void load_dots();
void render_dots(SDL_Rect *camera, SDL_Renderer *renderer);
void move_dots(SDL_Renderer*);
void update_dots_moves();
void before_game_loop();
void Clean();
