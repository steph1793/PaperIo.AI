#include "MainPlayer.h"
#include "../Utils/Globals.h"



void MainPlayer::move()
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
	if (mVelY == 0 && mVelX ==0) direction = -1;
	rect->updateRect(this->mBox, direction);

}

void MainPlayer::render(SDL_Rect* camera, SDL_Renderer *renderer)
{
	gDotTexture.render(mBox.x - camera->x, mBox.y - camera->y, renderer);
}

void MainPlayer::SetCamera(SDL_Rect* camera)
{
	camera->x = (mBox.x + DOT_WIDTH / 2) - SCREEN_WIDTH / 2;
	camera->y = (mBox.y + DOT_HEIGHT / 2) - SCREEN_HEIGHT / 2;

	//Keep the camera in bounds
	if (camera->x < 0)
	{
		camera->x = 0;
	}
	if (camera->y < 0)
	{
		camera->y = 0;
	}
	if (camera->x > LEVEL_WIDTH - camera->w)
	{
		camera->x = LEVEL_WIDTH - camera->w;
	}
	if (camera->y > LEVEL_HEIGHT - camera->h)
	{
		camera->y = LEVEL_HEIGHT - camera->h;
	}
}