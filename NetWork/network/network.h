#ifndef __NETWORK_H__
#define __NETWORK_H__

#if defined(PLATFORM_WINDOWS)
#	pragma warning(disable : 4005)
#	include <winsock2.h>
#	include <pthread/pthread.h>
typedef int32_t socklen_t;
#else
#	include <netdb.h>
#	include <sys/types.h>
#	include <sys/socket.h>
#	include <netinet/in.h>
#	include <arpa/inet.h>
#	include <unistd.h>
#	include <errno.h>
typedef int SOCKET;
#define INVALID_SOCKET (SOCKET)(~0)
#endif


#endif