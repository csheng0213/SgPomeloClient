#ifndef __NETCLIENT_H__
#define __NETCLIENT_H__

#include "network.h"
class CNetClient {
public:
	CNetClient();
	virtual ~CNetClient();

	bool Connect(const char *addr, uint32_t port);
	void Disconnect();
	bool IsConnected() { return m_isConnected; }

	bool Send(const void *data, size_t size);
	bool SendPing(uint32_t flag);

	virtual void OnReceived(const void *data, size_t len) = 0;

protected:
	static void *_network_thread(void *_this);
	void *network_thread();

private:
	pthread_t m_thread;
	SOCKET m_socket;
	sockaddr_in m_addr;
	bool m_isConnected;
	bool m_isRunning;

	uint64_t m_expiredTime;
	uint64_t m_nextPingTime;
};

#endif