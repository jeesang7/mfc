#pragma once

// CConnectSocket 명령 대상

class CConnectSocket : public CSocket
{
public:
	CConnectSocket();
	virtual ~CConnectSocket();

	void OnClose(int nErrorCode);
	void OnReceive(int nErrorCode);
};


