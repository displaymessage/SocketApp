#include "SocketApp.h"
int main(int argv, char* argc[]){
	LX::SocketPara socketPara;
	socketPara.m_iSocketDomain = AF_INET;
	socketPara.m_iSocketType = SOCK_STREAM;
	socketPara.m_iSocketProtocol = 0;
	socketPara.m_iAddrDomain = AF_INET;
	socketPara.m_strIp = "127.0.0.1";
	socketPara.m_usPort = static_cast<unsigned short>(20000);
	LX::SocketServer socketServer(socketPara);
	socketServer.Socket();
	socketServer.Bind();
}
