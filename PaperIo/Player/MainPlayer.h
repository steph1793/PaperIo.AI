#pragma once
#include "Player.h"

class MainPlayer : public Player {

public:
	bool set = false;
	virtual void move();
	void render(SDL_Rect *camera, SDL_Renderer *renderer);
	void MainPlayer::SetCamera(SDL_Rect*);
};