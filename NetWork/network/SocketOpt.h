#ifndef __SOCKETOPT_H__
#define __SOCKETOPT_H__

#include "network.h"
//#include "../Server/Net/SocketAPI.h"
#if defined(PLATFORM_WINDOWS)
#  define ERROR_WOULDBLOCK WSAEWOULDBLOCK
#  define ERROR_INPROGRESS WSAEINPROGRESS
//#elif defined (PLATFORM_IOS) || defined (PLATFORM_ANDROID) || defined(CC_TARGET_OS_IPHONE) || defined(PLATFORM_MAC)
#else

#  ifndef SOCKET_ERROR
#    define SOCKET_ERROR -1
#  endif
#  define ERROR_WOULDBLOCK EWOULDBLOCK
#  define ERROR_INPROGRESS EINPROGRESS
#endif
namespace SocketOpt
{
	// Get local ip
	PUB_API in_addr GetLocalIP();

	// Create file descriptor for socket i/o operations.
	SOCKET CreateTCPFileDescriptor();

	// Disable blocking send/recv calls.
	bool Nonblocking(SOCKET fd);

	// Enable blocking send/recv calls.
	bool Blocking(SOCKET fd);

	// Disable nagle buffering algorithm
	bool DisableBuffering(SOCKET fd);

	// Enables nagle buffering algorithm
	bool EnableBuffering(SOCKET fd);

	// Set internal buffer size to socket.
	bool SetRecvBufferSize(SOCKET fd, uint8_t size);

	// Set internal buffer size to socket.
	bool SetSendBufferSize(SOCKET fd, uint8_t size);

	// Set timeout, in seconds
	bool SetTimeout(SOCKET fd, int32_t timeoutS);

	uint32_t GetRecvTimeout(SOCKET fd);

	uint32_t GetSendTimeout(SOCKET fd);

	// Closes socket completely.
	void CloseSocket(SOCKET fd);

	// Sets SO_REUSEADDR
	void ReuseAddr(SOCKET fd);

	uint32_t	GetErrorNumber();
	uint32_t	GetSocketError(SOCKET fd);
};

#endif