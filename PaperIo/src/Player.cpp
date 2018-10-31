
#include <Player.h>
#include <Utils.h>

Player::Player()
{
	//Initialize the offsets
	mBox.x = 0;
	mBox.y = 0;
	//Initialize the velocity
	mVelX = 0;
	mVelY = 0;
	mBox.w = DOT_WIDTH;
	mBox.h = DOT_HEIGHT;

	
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
			break; 
		case SDLK_DOWN: 
			token = "DOWN";
			break;
		case SDLK_LEFT:
			token = "LEFT";
			break;
		case SDLK_RIGHT: 
			token = "RIGHT";
			break;
		}
		if (client_set && token != "")
		client->Send(to_string( client->id) + " "+ token );
		token = "";
	}
}

void Player::move(SDL_Renderer *renderer)
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
	int direction = (mVelX == 0) ? 1 : 0;
	if (mVelY == 0 && mVelX == 0) direction = -1;
	rect->updateRect(this->mBox, direction);
}

void Player::render(SDL_Rect *camera)
{
	if (checkCollision(camera,&mBox )) {
		//Show the dot
		gDotTexture.render(mBox.x-camera->x, mBox.y-camera->y, Renderer);
	}
}

