#include "SocketApp.h"
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
LX::SocketPara::SocketPara():m_iSocketDomain(0),m_iSocketType(0),m_iSocketProtocol(0),m_iAddrDomain(0),m_strIp(""),m_usPort(0),m_iListenNum(0){}
LX::SocketPara::SocketPara(const SocketPara& that){
	m_iSocketDomain = that.m_iSocketDomain;
	m_iSocketType = that.m_iSocketType;
	m_iSocketProtocol = that.m_iSocketProtocol;
	m_iAddrDomain = that.m_iAddrDomain;
	m_strIp = that.m_strIp;
	m_usPort = that.m_usPort;
	m_iListenNum = that.m_iListenNum;
}
LX::SocketPara& LX::SocketPara::operator=(const SocketPara& that){
	m_iSocketDomain = that.m_iSocketDomain;
	m_iSocketType = that.m_iSocketType;
	m_iSocketProtocol = that.m_iSocketProtocol;
	m_iAddrDomain = that.m_iAddrDomain;
	m_strIp = that.m_strIp;
	m_usPort = that.m_usPort;
	m_iListenNum = that.m_iListenNum;
	return *this;
}
LX::SocketPara::~SocketPara(){}
LX::Int LX::SocketPara::GetSockaddr(struct sockaddr_in* servaddr){
	servaddr->sin_family = m_iAddrDomain;
	int iRet = inet_pton(m_iAddrDomain, m_strIp.c_str(),&(servaddr->sin_addr));
	if(0 == iRet) std::cout<< m_strIp << " is invaild!" << std::endl;
	if(-1 == iRet){
		std::cout<< "AddrDomain: " << m_iAddrDomain << " is invaild! " << strerror(errno) << "errno: " << errno <<" EAFNOSUPPORT: " << EAFNOSUPPORT << std::endl;
	}
	servaddr->sin_port = htons(m_usPort);
}
LX::SocketApp::SocketApp(){
	m_iSocketFd = 0;
	m_iSocketDomain = 0;
	m_iSocketType = 0;
	m_iSocketProtocol = 0;
	m_bSocketFlag = false;
	std::cout<< "LX::SocketApp::SocketApp() is exec." << std::endl;
}

LX::SocketApp::SocketApp(SocketPara& socketPara):m_iSocketDomain(socketPara.m_iSocketDomain),m_iSocketType(socketPara.m_iSocketType),m_iSocketProtocol(socketPara.m_iSocketProtocol){
	m_iSocketFd = 0;
	m_bSocketFlag = false;
	std::cout << "LX::SocketApp::SocketApp(SocketPara& socketPara) is exec." << std::endl;
}

LX::SocketApp::~SocketApp(){
	CloseFd();
	std::cout << "LX::SocketApp::~SocketApp() is exec." << std::endl;
}

LX::Int LX::SocketApp::Socket(){
	if(m_bSocketFlag)
		return LX_OK;
	m_bSocketFlag = true;
	std::cout << "socket function exec now." << std::endl;
	m_iSocketFd = socket(m_iSocketDomain, m_iSocketType, m_iSocketProtocol);
	if(m_iSocketFd <= 0)
	{
		std::cout << "socket function is fail! " << strerror(errno) << std::endl;
		return LX_FAIL;
	}
	std::cout << "socket function is successful! " << std::endl;
	return LX_OK;
}

LX::Int LX::SocketApp::Socket(SocketPara& socketPara){
	if(m_bSocketFlag)
		return LX_OK;
	m_iSocketDomain = socketPara.m_iSocketDomain;
	m_iSocketType = socketPara.m_iSocketType;
	m_iSocketProtocol = socketPara.m_iSocketProtocol;
	Socket();
}

void LX::SocketApp::CloseFd(){
	std::cout << "socketfd is destroy!" << std::endl;
	close(m_iSocketFd);
}

LX::SocketApp::SocketApp(const SocketApp& that){}

LX::SocketApp& LX::SocketApp::operator=(const SocketApp& that){}

LX::SocketServer::SocketServer(SocketPara& socketPara):SocketApp(socketPara),m_cSocketPara(socketPara),m_bBindFlag(false){
	std::cout << "LX::SocketApp::SocketServer(SocketPara& socketPara) is exec." << std::endl;
}

LX::SocketServer::SocketServer():m_bBindFlag(false){
	std::cout << "LX::SocketApp::SocketServer() is exec." << std::endl;
}

LX::SocketServer::~SocketServer(){
	std::cout << "LX::SocketServer::~SocketServer() is exec." << std::endl;
}

LX::Int LX::SocketServer::Bind(){
	if(m_bBindFlag)
		return LX_OK;
	struct sockaddr_in servaddr;
	m_cSocketPara.GetSockaddr(&servaddr);
	if(-1 == bind(m_iSocketFd,(struct sockaddr*)&servaddr,sizeof(servaddr))){
		std::cout << "LX::SocketServer::Bind() is fail! " << strerror(errno) << std::endl;
		return LX_FAIL;
	}
	std::cout << "LX::SocketServer::Bind() is sucessful!" << std::endl;
	m_bBindFlag = true;
	return LX_OK;
}

LX::Int LX::SocketServer::Bind(SocketPara& socketPara){
	if(m_bBindFlag)
		return LX_OK;
	m_cSocketPara = socketPara;
	return Bind();
}

LX::Int LX::SocketServer::Listen(){
	if(-1 == listen(m_iSocketFd, m_cSocketPara.m_iListenNum)){
		std::cout << "LX::SocketServer::Listen() is fail! " << strerror(errno) << std::endl;
		return LX_FAIL;
	}
	std::cout << "LX::SocketServer::Listen() is successful! " << std::endl;
	return LX_OK;
}

LX::Int LX::SocketServer::Accept(){
	struct sockaddr clientaddr;
	socklen_t clientaddrLength;
	Int iFd = accept(m_iSocketFd, &clientaddr, &clientaddrLength);
	if(-1 == iFd){
		std::cout << "LX::SocketServer::Accept() is fail! " << strerror(errno) << std::endl;
		return LX_FAIL;
	}
	return iFd;
}

LX::SocketServer::SocketServer(const SocketServer& that){}

LX::SocketServer& LX::SocketServer::operator=(const SocketServer& that){}
