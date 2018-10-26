#include "stdafx.h"
#include "LButton.h"
#include "LTexture.h"


#include <iostream>
using namespace std;

LButton::LButton(LButtonSprite button)
{
	mPosition.x = 0;
	mPosition.y = 0;
	buttonType = button;

}
LButton::LButton()
{
	mPosition.x = 0;
	mPosition.y = 0;

}


void LButton::setPosition(int x, int y)
{
	mPosition.x = x;
	mPosition.y = y;
}

void LButton::handleEvent(SDL_Event* e, int  BUTTON_WIDTH, int BUTTON_HEIGHT)
{
	//If mouse event happened
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);

		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if (x < mPosition.x)
		{
			inside = false;
		}
		//Mouse is right of the button
		else if (x > mPosition.x + BUTTON_WIDTH)
		{
			inside = false;
		}
		//Mouse above the button
		else if (y < mPosition.y)
		{
			inside = false;
		}
		//Mouse below the button
		else if (y > mPosition.y + BUTTON_HEIGHT)
		{
			inside = false;
		}

		//Mouse is inside button
		if(inside)
		{

			//Set mouse over sprite
			switch (e->type)
			{
			case SDL_MOUSEBUTTONDOWN:
				if (this->buttonType == start) Start = true;
				break;

			}
		}
	}
}

void LButton::render(SDL_Renderer *renderer)
{
	//Show current button sprite
	gButtonSpriteSheetTexture.render(mPosition.x, mPosition.y,renderer);
}
