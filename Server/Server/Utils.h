#pragma once


#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "LButton.h"

#include "stdafx.h"
#include <iostream>
#include <SDL.h>
#include <SDL_net.h>
#include <cstring>
#include <csignal>
#include <string>
#include <sstream>
#include <time.h>
#include <vector>
#include <sstream>
#include <map> 


using namespace std; 

//Screen dimension constants
extern const int SCREEN_WIDTH ;
extern const int SCREEN_HEIGHT;

//Button constants
extern const int BUTTON_WIDTH ;
extern const int BUTTON_HEIGHT ;



//The window we'll be rendering to
extern SDL_Window* gWindow;

//The window renderer
extern SDL_Renderer* gRenderer;


extern LButton gButton;



extern bool running ;
extern bool connected ;



extern const unsigned short PORT;
extern const unsigned short BUFFER_SIZE ;
extern const unsigned short MAX_SOCKETS ;
extern const unsigned short MAX_CLIENTS ;

extern const string SERVER_NOT_FULL ;
extern const string SERVER_FULL ;

extern IPaddress  serverIp;
extern TCPsocket serverSocket;
extern TCPsocket clientSocket[];
extern bool socketIsFree[];
extern SDLNet_SocketSet socketSet;

extern char buffer[];
extern char global_buffer[];
extern int receivedByte_count;
extern int clientcount ;
extern bool shutdownServer ;

extern map<int, string> connected_clients;


//Starts up SDL and creates window
 bool init();
 bool Init_server();
 bool Send_to_all(string msg);
 void loop();
//Loads media
 bool loadMedia();

//Frees media and shuts down SDL
 void close();
 