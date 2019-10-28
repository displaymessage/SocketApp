#ifndef __SOCKET_APP_H__
#define __SOCKET_APP_H__
#include <sys/types.h>
#include <sys/socket.h>
namespace LX{
	typedef Int int;
	Int LX_OK = 0;
	Int LX_FAIL = -1;
	class SocketApp{
		public:
			SocketApp();
			SocketApp(Int iSocketDomain, Int iSocketType, Int iSocketProtocol):m_iSocketDomain(iSocketDomain),m_iSocketType(iSocketType),m_iSocketProtocol(iSocketProtocol);
			~SocketApp();
			Int Socket();
			Int Socket(Int iSocketDomain, Int iSocketType, Int iSocketProtocol);
			void CloseFd();
		private:
		public:
		private:
			Int m_iSocketFd;
			Int m_iSocketDomain;
			Int m_iSocketType;
			Int m_iSocketProtocol;
			bool m_bSocketFlag;
	};
};
#endif
