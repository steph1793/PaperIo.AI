#ifndef H_CLIENT
#define H_CLIENT

#include <iostream>
#include <SDL_net.h>
#include <cstring>
#include <string>
#include "../Utils/PlayersUtils.h"
using namespace std;

class Client{

	private:
		const unsigned short PORT = 1234;
		const unsigned short BUFFER_SIZE = 1000;
	

		IPaddress serverIp;
		TCPsocket clientSocket;
		SDLNet_SocketSet socketSet;
		string serverName = "localhost";

		string userInput = "";
		int inputLength= 0;
		char buffer[1000];
		const char *host = NULL;
	public:
		int id;
		bool shutdown = true;

	public:
		Client(){}
		Client(string servername);
		~Client(){if(host) delete host;}

		bool Init();
		string GetResponse();
		bool Send(string msg);
		void Clean();


};


#endif
