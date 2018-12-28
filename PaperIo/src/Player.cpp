
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

	kill();
	if (dead) return;

	int direction = (mVelX == 0) ? 1 : 0;
	if (mVelY == 0 && mVelX == 0) direction = -1;
	rect->updateRect(this->mBox, direction);
	Sint16 * vx = &(this->zone->vecx[0]);
	Sint16 * vy = &(this->zone->vecy[0]);
	int taille = this->zone->vecx.size();
	bool res = is_inside_polygon(mBox.x, mBox.y, vx, vy, taille);

	in = res ? true : false;
	out = res ? false : true;

	buffer_in.push(new Point(mBox.x, mBox.y, in));
	if (buffer_in.size() > 2) buffer_in.pop();


	if (buffer_in.size() == 2) {
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
			for (auto it = Dots.begin(); it != Dots.end(); it++) {
				if (this->id != (*it).second->id)
				{
					update_polygon_adverse(it->second->zone->vecx, it->second->zone->vecy, zone->vecx, zone->vecy);
				}
			}
			if (this->id != dot->id) {
				update_polygon_adverse(dot->zone->vecx, dot->zone->vecy, zone->vecx, zone->vecy);
			}
			for (auto it = rects_trail.begin(); it != rects_trail.end(); ) {
				if ((*it)->playerId == id) {
					it = rects_trail.erase(it);
				}
				else {
					it++;
				}
			}
			rect = new Rect(id, { mBox.x, mBox.y, mBox.w, mBox.h }, 0);
			rect->color = color;
			rects_trail.push_back(rect);




			temp_x.clear();
			temp_y.clear();
			delete p_in;
			delete p_out;
			p_in = NULL;
			p_out = NULL;
		}
	}
}

void Player::render(SDL_Rect *camera)
{
	if (checkCollision(camera,&mBox )) {
		//Show the dot
		gDotTexture.render(mBox.x-camera->x, mBox.y-camera->y, Renderer);
	}
}

void Player::kill() {
	vector<Rect*>tmp;
	for (auto rect = rects_trail.begin(); rect != rects_trail.end(); rect++) {
		if (checkCollision(&mBox, &(*rect)->box) && this->rect->box.x != (*rect)->box.x && this->rect->box.y != (*rect)->box.y) {
			if ((is_inside_polygon(mBox.x, mBox.y, &zone->vecx[0], &zone->vecy[0], zone->vecx.size()) && (*rect)->playerId == id) ||
				(id != dot->id && is_inside_polygon(mBox.x, mBox.y, &dot->zone->vecx[0], &dot->zone->vecy[0], dot->zone->vecx.size()) && (*rect)->playerId == dot->id) ||
				(Dots.size() > 0 && (*rect)->playerId != dot->id && is_inside_polygon(mBox.x, mBox.y, &Dots.at((*rect)->playerId)->zone->vecx[0], &Dots.at((*rect)->playerId)->zone->vecy[0], Dots.at((*rect)->playerId)->zone->vecx.size())))
				continue;
			if ((*rect)->playerId == dot->id) {
				dot->dead = true;
				return;
			}
			else {
				Dots.at((*rect)->playerId)->dead = true;
				for (auto it = rects_trail.begin(); it != rects_trail.end(); it++) {
					if ((*it)->playerId == (*rect)->playerId) {

						tmp.push_back(*it);
					}
				}

			}
		}
		
	}


	for (auto it = rects_trail.begin(); it != rects_trail.end(); ) {
		if (find(tmp.begin(), tmp.end(), *it) != tmp.end()) {
			it = rects_trail.erase(it);
		}
		else it++;
	}
	rects[0] = { 0 ,100 ,LEVEL_WIDTH,10 }; rects[1] = { LEVEL_WIDTH - 110 ,100 ,10,LEVEL_HEIGHT }; rects[2] = { 100 ,LEVEL_HEIGHT - 110 ,LEVEL_WIDTH,10 }; rects[3] = { 100 ,0,10,LEVEL_HEIGHT };

	for each ( auto r in rects)
	{
		if (checkCollision(&mBox, &r)) {
			if (id == dot->id) {
				dead = true; return;
			}
			else {
				for (auto it = rects_trail.begin(); it != rects_trail.end(); ) {
					if ((*it)->playerId == id) {
						it = rects_trail.erase(it);
					}
					else it++;
				}

				Dots.at(id)->dead=true;
				break;
			}
		}
	}
	
}