#include "SocketApp.h"
int main(int argv, char* argc[]){
	LX::SocketServer socketServer;
	socketServer.Socket(AF_INET, SOCK_STREAM, 0);
}
