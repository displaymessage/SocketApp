#include "SocketApp.h"
#include <errno.h>
#include <unistd.h>
#include <string.h>
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
LX::Int LX::SocketPara::GetSockaddr(struct sockaddr_storage* servaddr){
	switch(m_iAddrDomain){
		case AF_INET:
			{
				struct sockaddr_in* psockaddr_in = (struct sockaddr_in*)servaddr;
				psockaddr_in->sin_family = AF_INET;
				int iRet = inet_pton(AF_INET, m_strIp.c_str(),&(psockaddr_in->sin_addr));
				if(0 == iRet) std::cout<< m_strIp << " is invaild!" << std::endl;
				if(-1 == iRet){
					std::cout<< "AddrDomain: " << m_iAddrDomain << " is invaild! " << strerror(errno) << "errno: " << errno <<" EAFNOSUPPORT: " << EAFNOSUPPORT << std::endl;
				}
				psockaddr_in->sin_port = htons(m_usPort);
				break;
			}
		default:
			std::cout << "support AF_INET" << std::endl;
			break;
	}
	return LX_OK;
}
LX::SocketApp::SocketApp(){
	m_iSocketFd = 0;
	m_iSocketDomain = 0;
	m_iSocketType = 0;
	m_iSocketProtocol = 0;
	m_bSocketFlag = false;
	m_strReadBuf = "";
	m_iCurReadPosition = 0;
	m_iReadBufSize = 1024;
	std::cout<< "LX::SocketApp::SocketApp() is exec." << std::endl;
}

LX::SocketApp::SocketApp(SocketPara& socketPara):m_iSocketDomain(socketPara.m_iSocketDomain),m_iSocketType(socketPara.m_iSocketType),m_iSocketProtocol(socketPara.m_iSocketProtocol){
	m_iSocketFd = 0;
	m_bSocketFlag = false;
	m_strReadBuf = "";
	m_iCurReadPosition = 0;
	m_iReadBufSize = 1024;
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

LX::Int LX::SocketApp::Read(char* buf, Int iSocketFd){
	if(NULL == buf){
		std::cout << "LX::SocketApp::Read(void* buf, Int iLength, Int iSocketFd = 0) is fail! buf is null!" << std::endl; 
		return LX_FAIL;
	}
	std::cout << "5" << std::endl;
	Int iRet = LX_OK;
	if(m_iCurReadPosition < m_strReadBuf.length() && 0 != m_strReadBuf.length()){
		std::cout << "1" << std::endl;
		*buf = m_strReadBuf[m_iCurReadPosition];
		++m_iCurReadPosition;
		iRet = 1;
		std::cout << "LX::SocketApp::Read(void* buf, Int iLength, Int iSocketFd = 0) is exec! date from readBuf." << std::endl;
	}
	else{
		std::cout << "6" << std::endl;
		char szBuf[m_iReadBufSize];
		bzero(szBuf, m_iReadBufSize);
		if(0 == iSocketFd)
			iSocketFd = m_iSocketFd;
		void* pbuf = szBuf;
		Int iBufSize = m_iReadBufSize;
		Int iRead = 0;
		while((0 < (iRead = read(iSocketFd, pbuf, iBufSize)))){
			iBufSize = iBufSize - iRead;
			pbuf = szBuf + iRead;
		}
		if(iBufSize == m_iReadBufSize){
			m_strReadBuf = "";
			m_iCurReadPosition = 0;
			if(0 == iRead){
				std::cout << "LX::SocketApp::Read(void* buf, Int iLength, Int iSocketFd = 0) is exec! fd no date." << std::endl;
				iRet = LX_OK;
			}
			else{
				std::cout << "LX::SocketApp::Read(void* buf, Int iLength, Int iSocketFd = 0) is fail! " << strerror(errno) << std::endl;
				iRet = LX_FAIL;
			}
		}
		else{
			std::cout << "2" << std::endl;
			m_strReadBuf = szBuf;
			m_iCurReadPosition = 0;
			*buf = m_strReadBuf[m_iCurReadPosition];
			++m_iCurReadPosition;
			iRet = 1;
			std::cout << "LX::SocketApp::Read(void* buf, Int iLength, Int iSocketFd = 0) is exec! date from fd." << std::endl;
		}
	}
	return iRet;
}

LX::Int LX::SocketApp::Write(void* buf, Int iLength, Int iSocketFd){
	if(NULL == buf || 1 >= iLength){
		std::cout << "LX::SocketApp::Write(void* buf, Int iLength, Int iSocketFd) is fail! buf is null or iLength <= 1!" << std::endl;
		return LX_FAIL;
	}
	if(0 == iSocketFd)
		iSocketFd = m_iSocketFd;
	void* pBuf = buf;
	Int iBufSize = iLength;
	Int iWrite = 0;
	do{
		iWrite = write(iSocketFd, pBuf, iBufSize);
		if(-1 == iWrite){
			std::cout << "LX::SocketApp::Write(void* buf, Int iLength, Int iSocketFd) is fail! " << strerror(errno) << std::endl;
			return LX_FAIL;
		}
		pBuf = buf + iWrite;
		iBufSize = iBufSize - iWrite;
	}while(iWrite);
	std::cout << "LX::SocketApp::Write(void* buf, Int iLength, Int iSocketFd) is exec." << std::endl;
	return LX_OK;
}

LX::Int LX::SocketApp::ReadLine(void* buf, Int iBufSize, Int iSocketFd){
	if(NULL == buf || 1 >= iBufSize){
		std::cout << "LX::SocketApp::ReadLine(void* buf, Int iBufSize, Int iSocketFd) is fail! buf is null or iBufSize <= 1! " << std::endl;
		return LX_FAIL;
	}
	std::cout << "3" << std::endl;
	char* pBuf = static_cast<char*>(buf);
	char* pCurBuf = pBuf;
	bzero(buf, iBufSize);
	Int iResult = LX_OK;
	std::cout << "4" << std::endl;
	for(Int iLength = 1; iLength < iBufSize; ++iLength, ++pCurBuf){
		if(1 == (iResult = Read(pCurBuf, iSocketFd))){
			if('\n' == *(pBuf + iLength - 1))
				return iLength;
		}
		else if(LX_OK == iResult){
			return iLength - 1;
		}
		else
			return LX_FAIL;
	}
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
	struct sockaddr_storage servaddr;
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
	std::cout << "LX::SocketServer::Accept() is exec! " << std::endl;
	return iFd;
}

LX::SocketServer::SocketServer(const SocketServer& that){}

LX::SocketServer& LX::SocketServer::operator=(const SocketServer& that){}

LX::SocketClient::SocketClient(){
	std::cout << "LX::SocketClient::SocketClient() is exec!" << std::endl;
}

LX::SocketClient::SocketClient(SocketPara& socketPara):SocketApp(socketPara),m_cSocketPara(socketPara){
	std::cout << "LX::SocketClient::SocketClient(SocketPara& socketPara) is exec!" << std::endl;
}

LX::SocketClient::~SocketClient(){
	std::cout << "LX::SocketClient::~SocketClient() is exec!" << std::endl;
}

LX::SocketClient::SocketClient(const SocketClient& that){}

LX::SocketClient& LX::SocketClient::operator=(const SocketClient& that){}

LX::Int LX::SocketClient::Connect(struct sockaddr_storage& servaddr){
	if(-1 == connect(m_iSocketFd, (struct sockaddr*)&servaddr, sizeof(servaddr))){
		std::cout << "LX::SocketClient::Connect(struct sockaddr_in& servaddr) is fail! " << strerror(errno) << std::endl;
		return LX_FAIL;
	}
	std::cout << "LX::SocketClient::Connect(struct sockaddr_in& servaddr) is exec!" << std::endl;
	return LX_OK;
}
