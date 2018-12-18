#pragma once
#include <SDL.h>
#include "LTexture.h"
#include "Rectangle.h"
#include <Polygon.h>



class Player
{
public:
	//The dimensions of the dot
	static const int DOT_WIDTH = 20;
	static const int DOT_HEIGHT = 20;

	//Maximum axis velocity of the dot
	static const int DOT_VEL = 5;
	int color = 0;

	Rect* rect = NULL;
	Polygon *zone;
	//Initializes the variables
	Player();
	Player::Player(int Id, int x, int y);
		//Takes key presses and adjusts the dot's velocity
	void handleEvent(SDL_Event& e);

	//Moves the dot
	virtual void move(SDL_Renderer*);

	//Shows the dot on the screen
	void render(SDL_Rect*);

	int id;

	//The X and Y offsets of the dot
	SDL_Rect mBox;

	//The velocity of the dot
	int mVelX, mVelY;

	LTexture gDotTexture;

	bool in = true;
	bool out = false;

	queue<Point*> buffer_in;

	vector<int> temp_x;
	vector<int> temp_y;

	Point * p_in = NULL;
	Point * p_out = NULL;


};