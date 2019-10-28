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
	m_iSocketFd = socket(m_iSocketDomain, m_iSocketType, m_iSocketProtocol);
	if(m_iSocketFd <= 0)
		return -1;
	return LX_OK;
}

LX::Int LX::SocketApp::Socket(Int iSocketDomain, Int iSocketType, Int iSocketProtocol){
	if(m_bSocketFlag)
		return LX_OK;
	m_iSocketDomain = iSocketDomain;
	m_iSocketType = iSocketType;
	m_iSocketProtocol = iSocketProtocol;
	Socket();
}

void LX::SocketApp::CloseFd(){
}
