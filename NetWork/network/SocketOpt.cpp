#include "pub.h"
#include "SocketOpt.h"

#ifdef  PLATFORM_IOS
#include "netdb.h"
#endif
namespace SocketOpt
{
	// Get local ip
	in_addr GetLocalIP()
	{
		char buff[256] = { 0 };
		gethostname(buff, sizeof(buff));
		hostent *host = gethostbyname(buff);
		return **(in_addr **)host->h_addr_list;
	}

#if defined(PLATFORM_WINDOWS)

	// Create file descriptor for socket i/o operations.
	SOCKET CreateTCPFileDescriptor()
	{
		// create a socket for use with overlapped i/o.
		return ::socket(AF_INET, SOCK_STREAM, 0);
	}

	// Disable blocking send/recv calls.
	bool Nonblocking(SOCKET fd)
	{
		u_long arg = 1;
		return (::ioctlsocket(fd, FIONBIO, &arg) == 0);
	}

	// Disable blocking send/recv calls.
	bool Blocking(SOCKET fd)
	{
		u_long arg = 0;
		return (ioctlsocket(fd, FIONBIO, &arg) == 0);
	}

	// Disable nagle buffering algorithm
	bool DisableBuffering(SOCKET fd)
	{
		int arg = 1;
		return (setsockopt(fd, 0x6, TCP_NODELAY, (const char*)&arg, sizeof(arg)) == 0);
	}

	// Enable nagle buffering algorithm
	bool EnableBuffering(SOCKET fd)
	{
		int arg = 0;
		return (setsockopt(fd, 0x6, TCP_NODELAY, (const char*)&arg, sizeof(arg)) == 0);
	}

	// Set internal buffer size to socket.
	bool SetSendBufferSize(SOCKET fd, int size)
	{
		return (setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (const char*)&size, sizeof(size)) == 0);
	}

	// Set internal buffer size to socket.
	bool SetRecvBufferSize(SOCKET fd, int size)
	{
		return (setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (const char*)&size, sizeof(size)) == 0);
	}

	// Set internal timeout.
	bool SetTimeout(SOCKET fd, int32_t timeoutS)
	{
		struct timeval to;
		to.tv_sec = timeoutS;
		to.tv_usec = 0;
		if (setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (const char*)&to, sizeof(to)) != 0) return false;
		return (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&to, sizeof(to)) == 0);
	}

	uint32_t GetRecvTimeout(SOCKET fd)
	{
		struct timeval to = { 0 };
		int32_t	nLen = sizeof(to) ;
		getsockopt(fd, SOL_SOCKET, SO_SNDTIMEO,(char*)&to,&nLen);
		return to.tv_sec;
	}

	uint32_t GetSendTimeout(SOCKET fd)
	{
		struct timeval to = { 0 };
		int32_t	nLen = sizeof(to) ;
		getsockopt(fd, SOL_SOCKET, SO_SNDTIMEO,(char*)&to,&nLen);
		return to.tv_sec;
	}

	// Closes a socket fully.
	void CloseSocket(SOCKET fd)
	{
		shutdown(fd, SO_LINGER);
		closesocket(fd);
	}

	// Sets reuseaddr
	void ReuseAddr(SOCKET fd)
	{
		int option = 1;
		setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&option, 4);
	}
	uint32_t	GetErrorNumber()
	{
		return GetLastError();
	}

	uint32_t	GetSocketError(SOCKET fd)
	{
		int32_t error =0;
		int32_t len = sizeof(error);
		getsockopt(fd, SOL_SOCKET, SO_ERROR,(char*)&error,&len);
		return error;
	}
#else
	// Create file descriptor for socket i/o operations.
	SOCKET CreateTCPFileDescriptor()
	{
		SOCKET retval = socket(AF_INET, SOCK_STREAM, 0);
#ifdef PLATFORM_IOS
		int on = 1;
		setsockopt(retval, SOL_SOCKET, SO_NOSIGPIPE, &on, sizeof(on));
#endif //IPHONEOS
		// create a socket for use with overlapped i/o.
		return retval;
	}

	// Disable blocking send/recv calls.
	bool Nonblocking(SOCKET fd)
	{
		int flags = 0;
		if ((flags = fcntl(fd, F_GETFL, 0)) >= 0)
		{
			return ( fcntl(fd, F_SETFL, flags | O_NONBLOCK) >= 0 );
		}else{
			return false;
		}
	}

	// Disable blocking send/recv calls.
	bool Blocking(SOCKET fd)
	{
		int flags = 0;
		if ((flags = fcntl(fd, F_GETFL, 0)) >= 0)
		{
			return ( fcntl(fd, F_SETFL, flags & (~O_NONBLOCK) ) >= 0 );
		}else{
			return false;
		}
	}

	// Disable nagle buffering algorithm
	bool DisableBuffering(SOCKET fd)
	{
		uint32_t arg = 1;
		return (setsockopt(fd, 0x6, 0x1, (const char*)&arg, sizeof(arg)) == 0);
	}

	// Enable nagle buffering algorithm
	bool EnableBuffering(SOCKET fd)
	{
		uint32_t arg = 0;
		return (setsockopt(fd, 0x6, 0x1, (const char*)&arg, sizeof(arg)) == 0);
	}

	// Set internal buffer size to socket.
	bool SetSendBufferSize(SOCKET fd, uint32_t size)
	{
		return (setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (const char*)&size, sizeof(size)) == 0);
	}

	// Set internal buffer size to socket.
	bool SetRecvBufferSize(SOCKET fd, uint32_t size)
	{
		return (setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (const char*)&size, sizeof(size)) == 0);
	}

	// Set internal timeout.
	bool SetTimeout(SOCKET fd, int32_t timeoutS)
	{
		struct timeval to;
		to.tv_sec = timeoutS;
		to.tv_usec = 0;
		if (setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (const char*)&to, (socklen_t)sizeof(to)) != 0) return false;
		return (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&to, (socklen_t)sizeof(to)) == 0);
	}

	uint32_t GetRecvTimeout(SOCKET fd)
	{
		struct timeval to = { 0 };
		socklen_t	nLen = sizeof(to) ;
		getsockopt(fd, SOL_SOCKET, SO_SNDTIMEO,(char*)&to,&nLen);
		return to.tv_sec;
	}

	uint32_t GetSendTimeout(SOCKET fd)
	{
		struct timeval to = { 0 };
		socklen_t	nLen = sizeof(to) ;
		getsockopt(fd, SOL_SOCKET, SO_SNDTIMEO,(char*)&to,&nLen);
		return to.tv_sec;
	}
	// Closes a socket fully.
	void CloseSocket(SOCKET fd)
	{
		shutdown(fd, SHUT_RDWR);
		close(fd);
	}

	// Sets reuseaddr
	void ReuseAddr(SOCKET fd)
	{
		uint32_t option = 1;
		setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&option, 4);
	}

	uint32_t GetErrorNumber()
	{
		return errno;
	}

	uint32_t GetSocketError(SOCKET fd)
	{
		int32_t error;
		socklen_t len = sizeof(error);
		getsockopt(fd, SOL_SOCKET, SO_ERROR,(char*)&error,&len);
		return error;
	}
#endif
}