#include <MainPlayer.h>
#include <Globals.h>


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