#include "stdafx.h"
#include "ChatServer.h"
#include "ListenSocket.h"
#include "ClientSocket.h"


CListenSocket::CListenSocket()
{
}

CListenSocket::~CListenSocket()
{
}

void CListenSocket::OnAccept(int nErrorCode) {
	CClientSocket* pClient = new CClientSocket;
	if (Accept(*pClient)) {
		pClient->SetListenSocket(this);
		m_ptrClientSocketList.AddTail(pClient);
	}
	else {
		delete pClient;
		AfxMessageBox(_T("Error: Failed to accept new client!"));
	}

	CAsyncSocket::OnAccept(nErrorCode);	
}

void CListenSocket::CloseClientSocket(CSocket* pClient) {
	POSITION pos;
	pos = m_ptrClientSocketList.Find(pClient);
	if (pos != NULL) {
		if (pClient != NULL) {
			pClient->ShutDown();
			pClient->Close();
		}

		m_ptrClientSocketList.RemoveAt(pos);
		delete pClient;
	}
}

void CListenSocket::SendChatDataAll(TCHAR* pszMessage) {
	POSITION pos;
	pos = m_ptrClientSocketList.GetHeadPosition();
	CClientSocket* pClient = NULL;

	while (pos != NULL) {
		pClient = (CClientSocket*)m_ptrClientSocketList.GetNext(pos);		// get and then move to position
		if (pClient != NULL) {
			pClient->Send(pszMessage, lstrlen(pszMessage) * 2);
		}
	}
}