
#include "Player.h"
#include "../Utils/Utils.h"

Player::Player()
{
	//Initialize the offsets
	mPosX = 0;
	mPosY = 0;

	//Initialize the velocity
	mVelX = 0;
	mVelY = 0;

	
}

Player::Player(int Id, int x, int y)
{
	//Initialize the offsets
	mPosX = x;
	mPosY = y;
	id = Id;
	//Initialize the velocity
	mVelX = 0;
	mVelY = 0;


}

void Player::handleEvent(SDL_Event& e)
{
	string token = "";
	//If a key was pressed
	if (e.type == SDL_KEYDOWN )
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: 
			token = "UP";
			//mVelY = -DOT_VEL;
			//mVelX = 0;
			break; 
		case SDLK_DOWN: 
			token = "DOWN";
			//mVelY = DOT_VEL; 
			//mVelX = 0;
			break;
		case SDLK_LEFT:
			token = "LEFT";
			//mVelX = -DOT_VEL; 
			//mVelY = 0;
			break;
		case SDLK_RIGHT: 
			token = "RIGHT";
			//mVelX = DOT_VEL;
			//mVelY = 0;
			break;
		}
		if (client_set && token != "")
		client->Send(to_string( client->id) + " "+ token );
		token = "";
	}
}

void Player::move()
{
	//Move the dot left or right
	mPosX += mVelX;

	//If the dot went too far to the left or right
	if ((mPosX < 0) || (mPosX + DOT_WIDTH > SCREEN_WIDTH))
	{
		//Move back
		mPosX -= mVelX;
	}    
	//Move the dot up or down
	mPosY += mVelY;

	//If the dot went too far up or down
	if ((mPosY < 0) || (mPosY + DOT_HEIGHT > SCREEN_HEIGHT))
	{
		//Move back
		mPosY -= mVelY;
	}
}

void Player::render()
{
	//Show the dot
	gDotTexture.render(mPosX, mPosY, Renderer);
}

