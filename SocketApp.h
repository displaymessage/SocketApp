#ifndef __SOCKET_APP_H__
#define __SOCKET_APP_H__
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
namespace LX{
#define LX_OK 0;
#define LX_FAIL -1;
	typedef int Int;
	class SocketPara{
		public:
			SocketPara();
			~SocketPara();
			SocketPara(const SocketPara& that);
			SocketPara& operator=(const SocketPara& that);
			Int GetSockaddr(struct sockaddr_in* servaddr);
		public:
			Int m_iSocketDomain;
			Int m_iSocketType;
			Int m_iSocketProtocol;
			Int m_iAddrDomain;
			std::string m_strIp;
			unsigned short m_usPort;
			Int m_iListenNum;
	};
	class SocketApp{
		public:
			SocketApp();
			SocketApp(SocketPara& socketPara);
			~SocketApp();
			Int Socket();
			Int Socket(SocketPara& socketPara);
			void CloseFd();
		private:
			SocketApp(const SocketApp& that);
			SocketApp& operator=(const SocketApp& that);
		protected:
			Int m_iSocketFd;
			Int m_iSocketDomain;
			Int m_iSocketType;
			Int m_iSocketProtocol;
			bool m_bSocketFlag;
	};
	class SocketServer:public SocketApp{
		public:
			SocketServer();
			SocketServer(SocketPara& socketPara);
			~SocketServer();
			Int Bind();
			Int Bind(SocketPara& socketPara);
			Int Listen();
			Int Accept();
		private:
			SocketServer(const SocketServer& that);
			SocketServer& operator=(const SocketServer& that);
		private:
			SocketPara m_cSocketPara;
			bool m_bBindFlag;
	};
	/*
	class SocketClient:public SocketAPP{
		public:
			SocketClient();
			SocketClient(Int iSocketDomain, Int iSocketType, Int iSocketProtocol);
			~SocketClient();
	};
	*/
};
#endif
