#include "SocketApp.h"
LX::SocketApp::SocketApp(){
	m_iSocketFd = 0;
	m_iSocketDomain = 0;
	m_iSocketType = 0;
	m_iSocketProtocol = 0;
	m_bSocketFlag = false;
}

LX::SocketApp::SocketApp(Int iSocketDomain, Int iSocketType, Int iSocketProtocol){
	m_iSocketFd = 0;
	m_bSocketFlag = false;
}

LX::SocketApp::~SocketApp(){
	CloseFd();
}

LX::Int LX::SocketApp::Socket(){
	if(m_bSocketFlag)
		return LX_OK;
	m_bSocketFlag = true;
}

LX::Int LX::SocketApp::Socket(Int iSocketDomain, Int iSocketType, Int iSocketProtocol){
	if(m_bSocketFlag)
		return LX_OK;
	m_bSocketFlag = true;
}

void LX::SocketApp::CloseFd(){
}
