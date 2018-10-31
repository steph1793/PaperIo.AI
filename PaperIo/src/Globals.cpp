#include <Globals.h>

const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;


const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;



const char* windowTitle = "SDL Tuto";


const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Renderer* Renderer = NULL;

TTF_Font *gFont = NULL;

Client *client = NULL;
bool client_set = false;


bool start = false;


MainPlayer* dot = new MainPlayer();
map<int, Player*> Dots;
vector<Rect*> rects_trail;