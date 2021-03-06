#include <client.h>

#pragma warning(disable: 4996)

Client::Client(string servername){
	this->serverName = servername.c_str();
}

bool Client::Init(){
	if(SDLNet_Init() <0){
		return false;
	}
	socketSet = SDLNet_AllocSocketSet(1);
	if(!socketSet){
		return false;
	}
	if(SDLNet_ResolveHost(&serverIp, serverName.c_str(), PORT) <0) return false;

	host = SDLNet_ResolveIP(&serverIp);
	if(!host){
		return false;
	}

	clientSocket = SDLNet_TCP_Open(&serverIp);
	if(!clientSocket) return false;
	SDLNet_TCP_AddSocket(socketSet, clientSocket);

	return true;

}


string Client::GetResponse(){
	string s="";
	int numActive = SDLNet_CheckSockets(socketSet,0);

	if(numActive >0){
		int messagefromServ = SDLNet_SocketReady(clientSocket);
		if(messagefromServ !=0){
			memset(buffer, '\0', BUFFER_SIZE);
			int response_count = SDLNet_TCP_Recv(clientSocket, buffer, BUFFER_SIZE);
			/// I process the response from the server
			s = buffer;
		}
	}
	return s;
}

bool Client::Send(string msg){
	memset(buffer, '\0', BUFFER_SIZE);
	strcpy(buffer, msg.c_str());
	return (SDLNet_TCP_Send(clientSocket, (void*)buffer, strlen(buffer)+1) < BUFFER_SIZE+1);
}

void Client::Clean(){
	SDLNet_TCP_Close(clientSocket);
	SDLNet_Quit();
}


