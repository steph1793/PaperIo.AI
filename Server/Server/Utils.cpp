#include "stdafx.h"
#include "Utils.h"
#include <random>
#include <algorithm>
#include <iostream>


using namespace std;

#pragma warning(disable: 4996)

const int SCREEN_WIDTH = 400;
const int SCREEN_HEIGHT = 200;

//Button constants
const int BUTTON_WIDTH = 416;
const int BUTTON_HEIGHT = 127;



//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

LButton gButton;


bool running = true;
bool connected = false;



const unsigned short PORT = 1234;
const unsigned short BUFFER_SIZE = 1000;
const unsigned short MAX_SOCKETS = 4;
const unsigned short MAX_CLIENTS = MAX_SOCKETS - 1;

const string SERVER_NOT_FULL = "OK";
const string SERVER_FULL = "FULL";

IPaddress  serverIp;
TCPsocket serverSocket;
TCPsocket clientSocket[MAX_CLIENTS];
bool socketIsFree[MAX_CLIENTS];
SDLNet_SocketSet socketSet;

char buffer[BUFFER_SIZE];
char global_buffer[BUFFER_SIZE];
int receivedByte_count = 0;
int clientcount = 0;
bool shutdownServer = false;

map<int, string> connected_clients;


vector<int> colors;


bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cout << "SDL could not initialize! SDL Error: "  << SDL_GetError() << endl;
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			cout << "Warning: Linear texture filtering not enabled!\n";
		}

		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			cout << "Window could not be created! SDL Error: " << SDL_GetError();
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << endl;
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load sprites
	if (!gButton.gButtonSpriteSheetTexture.loadFromFile("Media/Images/button.png", gRenderer))
	{
		cout << "Failed to load button sprite texture!\n";
		success = false;
	}
	else
	{
		gButton.setPosition(SCREEN_WIDTH - BUTTON_WIDTH, 100);
	}

	return success;
}



bool Init_server() {
	if (SDLNet_Init() == -1) {
		return false;
	}
	socketSet = SDLNet_AllocSocketSet(MAX_SOCKETS);
	if (!socketSet) return false;

	for (int i = 0; i<MAX_CLIENTS; i++) {
		clientSocket[i] = NULL;
		socketIsFree[i] = true;
	}

	if (SDLNet_ResolveHost(&serverIp, NULL, PORT) <0) return false;

	serverSocket = SDLNet_TCP_Open(&serverIp);
	if (!serverSocket) return false;

	SDLNet_TCP_AddSocket(socketSet, serverSocket);
	cout << "Initialisation reussie\n";
	return true;

}

int random_num(int min , int max)
{
	std::mt19937 rng;
	rng.seed(std::random_device()());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(min, max); // distribution in range [1, 6]

	return dist6(rng);
}

string join(char buff[], map<int, string> vec)
{
	stringstream ss;
	ss << buff << ";";
	for (auto i = vec.rbegin(); i != vec.rend(); i++)
		ss << i->second << ";";
	return ss.str();
}

bool Send_to_all(string msg) {
	memset(buffer, '\0', BUFFER_SIZE);
	strcpy(buffer, msg.c_str());
	for (int i = 0; i < clientcount; i++) {
		(SDLNet_TCP_Send(clientSocket[i], (void*)buffer, strlen(buffer) + 1) < BUFFER_SIZE + 1);
	}
	return true;
}
void loop() {

	int activeNum = SDLNet_CheckSockets(socketSet, 0);

	if (SDLNet_SocketReady(serverSocket)) {
		if (clientcount < MAX_CLIENTS) {
			int freespot = -100;
			for (int i = 0; i<MAX_CLIENTS; i++) {
				if (socketIsFree[i]) {
					socketIsFree[i] = false;
					freespot = i;
					break;
				}
			}
			clientSocket[freespot] = SDLNet_TCP_Accept(serverSocket);
			SDLNet_TCP_AddSocket(socketSet, clientSocket[freespot]);
			clientcount++;


			////// We send a message to the client to tell him he is connected
			memset(buffer, '\0', BUFFER_SIZE);
			int value = random_num(0, 7);
			while (std::find(colors.begin(), colors.end(), value) != colors.end()) {
				value = random_num(0, 7);
			}
			colors.push_back(value);
			sprintf(buffer, "%d connected %d %d %d", freespot, random_num(200,600), random_num(200,600), value );


			for (int i = 0; i < clientcount; i++)
			{
				if (i != freespot)
					SDLNet_TCP_Send(clientSocket[i], (void*)buffer, BUFFER_SIZE + 1);
			}
			string tmp = join(buffer, connected_clients);
			connected_clients.insert(pair<int, string>(freespot, buffer));

			memset(buffer, '\0', BUFFER_SIZE);
			strcpy(buffer, tmp.c_str());

			SDLNet_TCP_Send(clientSocket[freespot], (void*)buffer, BUFFER_SIZE + 1);

		}
		else {
			TCPsocket temp = SDLNet_TCP_Accept(serverSocket);

			/////// We send a message to say the room is full
			memset(buffer, '\0', BUFFER_SIZE);
			strcpy(buffer, "full");
			SDLNet_TCP_Send(temp, (void*)buffer, BUFFER_SIZE + 1);

			SDLNet_TCP_Close(temp);
		}
	}

	for (int i = 0; i<MAX_CLIENTS; i++) {
		if (SDLNet_SocketReady(clientSocket[i])) {
			memset(buffer, '\0', BUFFER_SIZE);
			receivedByte_count = SDLNet_TCP_Recv(clientSocket[i], buffer, BUFFER_SIZE);
			if (receivedByte_count <= 0) {
				SDLNet_TCP_DelSocket(socketSet, clientSocket[i]);
				SDLNet_TCP_Close(clientSocket[i]);
				clientSocket[i] = NULL;
				socketIsFree[i] = true;
				clientcount--;

				connected_clients.erase(connected_clients.find(i));
			}
			else {
				sprintf(global_buffer + strlen(global_buffer), "%s;", buffer);
				//printf("%s", global_buffer);
			}
		}
	}
	if (strlen(global_buffer) > 0) {

		for (int j = 0; j<MAX_CLIENTS; j++) {
			int len = strlen(buffer) + 1;
			if (len>1 && socketIsFree[j] == false) {
				//// send the message to other clients

				cout << "Retransmitting the message : " << buffer << endl;
				SDLNet_TCP_Send(clientSocket[j], (void*)buffer, strlen(buffer) + 1);
			}
		}
		memset(global_buffer, '\0', strlen(global_buffer));
	}
}



void close()
{
	SDLNet_FreeSocketSet(socketSet);
	SDLNet_TCP_Close(serverSocket);
	SDLNet_Quit(); 

	//Free loaded images
	gButton.gButtonSpriteSheetTexture.free();

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}
