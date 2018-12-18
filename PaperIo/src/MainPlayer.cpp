#include <MainPlayer.h>
#include <Globals.h>



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
	Sint16 * vx = &(this->zone->vecx[0]);
	Sint16 * vy = &(this->zone->vecy[0]);
	int taille = this->zone->vecx.size();
	bool res = is_inside_polygon(mBox.x, mBox.y, vx, vy, taille);

	in = res ? true : false;
	out = res ? false : true;

	buffer_in.push(new Point(mBox.x, mBox.y, in));
	if (buffer_in.size() > 2) buffer_in.pop();


	if (buffer_in.size()==2) {
		Point *a = buffer_in.front();
		Point *b = buffer_in.back();

		
		
		if (a->in == false && b->in == true) {
			p_in = find_in_out(a, b, vx, vy, taille);
		}
		if (a->in == true && b->in == false) {
			p_out = find_in_out(b, a, vx, vy, taille);
		}

		if (p_in && p_out) {


			this->temp_x.insert(temp_x.begin(), p_out->x);
			this->temp_x.insert(temp_x.end(), p_in->x);
			this->temp_y.insert(temp_y.begin(), p_out->y);
			this->temp_y.insert(temp_y.end(), p_in->y);
			update_polygon(zone->vecx, zone->vecy, temp_x, temp_y);

			temp_x.clear();
			temp_y.clear();
			delete p_in;
			delete p_out;
			p_in = NULL;
			p_out = NULL;
		}
	}

	


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