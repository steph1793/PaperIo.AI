
#include "Player.h"
#include "../Utils/Utils.h"

Player::Player()
{
	//Initialize the offsets
	mBox.x = 0;
	mBox.y = 0;



	//Initialize the velocity
	mVelX = 0;
	mVelY = 0;

	
}

Player::Player(int Id, int x, int y)
{
	//Initialize the offsets
	mBox.x = x;
	mBox.y = y;
	mBox.w = DOT_WIDTH;
	mBox.h = DOT_HEIGHT;
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
	mBox.x += mVelX;

	//If the dot went too far to the left or right
	if ((mBox.x < 0) || (mBox.x + DOT_WIDTH > LEVEL_WIDTH))
	{
		//Move back
		mBox.x -= mVelX;
	}    
	//Move the dot up or down
	mBox.y += mVelY;

	//If the dot went too far up or down
	if ((mBox.y < 0) || (mBox.y + DOT_HEIGHT > LEVEL_HEIGHT))
	{
		//Move back
		mBox.y -= mVelY;
	}
}

void Player::render(SDL_Rect *camera)
{
	if (checkCollision(camera,&mBox )) {
		//Show the dot
		gDotTexture.render(mBox.x-camera->x, mBox.y-camera->y, Renderer);
	}
}

