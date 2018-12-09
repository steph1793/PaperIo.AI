#pragma once
#include "LTexture.h"
#include "ButtonUtils.h"

//The mouse button
class LButton
{
public:
	//Initializes internal variables
	LButton(LButtonSprite);
	LButton();
	//Sets top left position
	void setPosition(int x, int y);

	//Handles mouse event
	void LButton::handleEvent(SDL_Event* e, int  BUTTON_WIDTH, int BUTTON_HEIGHT);

	//Shows button sprite
	void LButton::render(SDL_Renderer *renderer);
		LTexture gButtonSpriteSheetTexture;

		LButtonSprite buttonType;
private:
	//Top left position
	SDL_Point mPosition;

};
