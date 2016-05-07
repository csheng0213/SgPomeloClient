#include "pub.h"
#include "msghead.h"
#include "network.h"
#include "SocketOpt.h"
#include "NetClient.h"
#include "GlobalConfig.h"

static char send_buffer[MAX_MSG_LEN + sizeof(SMsgHead)];
static char recv_buffer[MAX_MSG_LEN + sizeof(SMsgHead)];
static char recv_buffer2[MAX_MSG_LEN << 4];

CNetClient::CNetClient()
{
#if defined(PLATFORM_WINDOWS)
	WSAData wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
#endif

	m_socket = INVALID_SOCKET;
	memset(&m_addr, 0, sizeof(m_addr));
	m_isConnected = false;
	m_isRunning = false;

	m_expiredTime = 0;
	m_nextPingTime = 0;
}

CNetClient::~CNetClient()
{
#if defined(PLATFORM_WINDOWS)
	WSACleanup();
#endif
}

bool CNetClient::Connect(const char *ip, uint32_t port)
{
	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip);
	addr.sin_port = htons((uint16_t)port);

	if (m_socket != INVALID_SOCKET) {
		if (memcmp(&m_addr, &addr, sizeof(sockaddr_in)) != 0)
			return false;
		return true;
	}

	m_socket = SocketOpt::CreateTCPFileDescriptor();
	m_addr = addr;

	pthread_create(&m_thread, NULL, _network_thread, this);
	return true;
}

void CNetClient::Disconnect()
{
	m_isRunning = false;
}

bool CNetClient::Send(const void *data, size_t size)
{
	if (data == NULL || size == 0)
		return false;
	
	if (m_socket == INVALID_SOCKET)
		return false;

	SMsgHead *pHead = (SMsgHead *)send_buffer;
	size_t _size = pub::LzwEncode(data, size, &pHead[1], MAX_MSG_LEN);
	if (_size > 0 && _size < size) {
		if (_size > MAX_MSG_LEN) return false;
		
		pHead->compressed = 1;
		size = _size;
	}
	else {
		if (size > MAX_MSG_LEN) return false;

		pHead->compressed = 0;
		memcpy(&pHead[1], data, size);
	}
	pHead->length = size;
	if (send(m_socket, send_buffer, size + sizeof(SMsgHead), 0) < 0)
		return false;

	return true;
}

//发送ping消息封装
bool CNetClient::SendPing(uint32_t flag)
{
	if (m_socket == INVALID_SOCKET)
		return false;

	SMsgHead *pHead = (SMsgHead *)send_buffer;
	pHead->length = 0;
	pHead->ping = flag;
	pHead->compressed = 0;

	if (send(m_socket, send_buffer, sizeof(SMsgHead), 0) < 0)
		return false;

	return true;
}

void *CNetClient::_network_thread(void *_this)
{
	CNetClient *client = (CNetClient *)_this;
	client->network_thread();
	return NULL;
}

void *CNetClient::network_thread()
{
	int32_t result = connect(m_socket, (const sockaddr *)&m_addr, sizeof(m_addr));
	if (result == -1) {
		SocketOpt::CloseSocket(m_socket);
		m_socket = INVALID_SOCKET;
		OnReceived(NULL, 1);
		m_isConnected = false;
		m_isRunning = false;
		return NULL;
	}

	SocketOpt::Nonblocking(m_socket);
	SocketOpt::DisableBuffering(m_socket);
	OnReceived(NULL, 0);
	m_isConnected = true;
	m_isRunning = true;

	m_expiredTime = PUB_get_cur_tick() + EXPIRED_TIME_CS;
	m_nextPingTime = PUB_get_cur_tick() + PING_INTERVAL_CS;

	bool recvHead = true;
	char *cur_buff = recv_buffer;
	int cur_len = 0;
	int total_len = sizeof(SMsgHead);

	while (m_isRunning) {

		if (GlobalConfig::sendPing == 1)
		{
			//过期检查
			uint64_t now = PUB_get_cur_tick();
			if (m_expiredTime && now >= m_expiredTime) {
				m_expiredTime = 0;
				m_nextPingTime = 0;
				SendPing(PROTO_BREAK_CS);
				PUB_log_file("log.txt", "------ send [break] sock:%d time: [%d] -------", (uint32_t)m_socket, (now));

				SocketOpt::CloseSocket(m_socket);
				m_socket = INVALID_SOCKET;
				OnReceived(NULL, 3);
				m_isConnected = false;
				m_isRunning = false;
				return NULL;
			}

			//send ping
			if (m_nextPingTime && now >= m_nextPingTime) {
				SendPing(PROTO_PING_CS);
				//PUB_log_file("log.txt", "send [ping] msg  time: [%d]", (now));

				m_expiredTime = now + EXPIRED_TIME_CS;
				m_nextPingTime = now + PING_INTERVAL_CS;
			}
		}

		fd_set rset;
		FD_ZERO(&rset);
		FD_SET(m_socket, &rset);
		timeval tm = { 0, 1000 };
		if (select(m_socket + 1, &rset, NULL, NULL, &tm) <= 0) {
			continue;
		}

		if (FD_ISSET(m_socket, &rset)) {
			bool error = false;

			uint64_t unow = PUB_get_cur_tick();
			m_expiredTime = unow + EXPIRED_TIME_CS;
			m_nextPingTime = unow + PING_INTERVAL_CS;

			while (true) {
				int nLen = total_len - cur_len;
				int n = recv(m_socket, cur_buff, nLen, 0);
				if (n == SOCKET_ERROR) {
					uint32_t err = SocketOpt::GetErrorNumber();
					if (err != ERROR_WOULDBLOCK) {
						error = true;
					}
					break;
				}
				else if (n == 0) {
					error = true;
					break;
				}
				cur_buff += n;
				cur_len += n;
				if (cur_len == total_len) {
					SMsgHead *pHead = (SMsgHead *)recv_buffer;
					
					//处理ping消息
					if (IsPongMsg(pHead)) {
						recvHead = true;
						cur_buff = recv_buffer;
						cur_len = 0;
						total_len = sizeof(SMsgHead);
						//PUB_log_file("log.txt", "rev [pong] msg  time: [%d]", (unow));
						break;
					}

					if (recvHead) {
						recvHead = false;
						cur_len = 0;
						total_len = pHead->length;
					}
					else {
						if (pHead->compressed) {
							size_t size = pub::LzwDecode(&pHead[1], pHead->length, recv_buffer2, sizeof(recv_buffer2));
							if (size > 0)
								OnReceived(recv_buffer2, size);
						}
						else	// not compressed, the data is original
							OnReceived(&pHead[1], pHead->length);

						recvHead = true;
						cur_buff = recv_buffer;
						cur_len = 0;
						total_len = sizeof(SMsgHead);
					}
				}
				if (n < nLen)
					break;
			}
			if (error)
				break;
		}

		

	}

	SocketOpt::CloseSocket(m_socket);
	m_socket = INVALID_SOCKET;
	OnReceived(NULL, 2);
	m_isConnected = false;
	m_isRunning = false;
	return NULL;
}