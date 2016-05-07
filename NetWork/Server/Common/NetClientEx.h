#ifndef __NETCLIENTEX_H__
#define __NETCLIENTEX_H__

#include "pub.h"
#include "mts_queue.h"
#include "NetClient.h"

class CNetClientEx : public CNetClient {
public:
	CNetClientEx();
	~CNetClientEx();

	virtual void OnConnected();
	virtual void OnConnectFailed();
	virtual void OnDisconnected();
	virtual void OnReceived(const void *data, size_t size);

	void PushSendMsg(const void *data, size_t size);

	template<class CB>
	void DispatchMsg(CB cb) {
		while (!m_MsgQueue.Empty()) {
			SNetMsg *pMsg = m_MsgQueue.Front();
			if (pMsg->data != NULL) {
				cb(pMsg->data, pMsg->len);
				delete [] pMsg->data;
			}
			else {
				if (pMsg->len == 0) {
					OnConnected();
				}
				else if (pMsg->len == 1) {
					OnConnectFailed();
				}
				else if (pMsg->len == 2) {
					OnDisconnected();
				}
				else if (pMsg->len == 3) {
					OnDisconnected();
				}
			}
			delete pMsg;
			m_MsgQueue.Pop();
		}
	}

private:
	void SendAllMsgInSendQueue();
	void RemoveAllMsgInSendQueue();

private:
	struct SNetMsg {
		void *data;
		size_t len;
	};
	std::queue<struct SNetMsg *> m_SendMsgQueue;
	mts_queue<struct SNetMsg *> m_MsgQueue;
};

#endif