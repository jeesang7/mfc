// ClientSocket.cpp: 구현 파일
//

#include "stdafx.h"
#include "ChatServer.h"
#include "ClientSocket.h"
#include "ListenSocket.h"
#include "ChatServerDlg.h"


// CClientSocket

CClientSocket::CClientSocket()
{
}

CClientSocket::~CClientSocket()
{
}

void CClientSocket::SetListenSocket(CAsyncSocket* pSocket) {
	m_pListenSocket = pSocket;
}

void CClientSocket::OnClose(int nErrorCode) {
	CSocket::OnClose(nErrorCode);

	CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
	pServerSocket->CloseClientSocket(this);
}

void CClientSocket::OnReceive(int nErrorCode) {
	CString strTmp = _T(""), strIPAdress = _T(""), strTime = _T("");
	UINT uPortNumber = 0;
	TCHAR szBuffer[1024], szGreet[1024];
	::ZeroMemory(szBuffer, sizeof(szBuffer));	
	wsprintf(szGreet, _T("hello"));			// Greeting message from client

	GetPeerName(strIPAdress, uPortNumber);
	if (Receive(szBuffer, sizeof(szBuffer)) > 0) {
		CChatServerDlg* pMain = (CChatServerDlg*)AfxGetMainWnd();
		COleDateTime curTime = COleDateTime::GetCurrentTime();
		strTime = curTime.Format(_T("[%H:%M:%S] "));

		if (lstrcmp(szBuffer, szGreet) == 0) {			// Greeting, enroll in client list.
			strTmp.Format(_T("[%s:%d]"), strIPAdress, uPortNumber);
			pMain->m_ListClient.AddString(strTime + strTmp);
			pMain->m_ListClient.SetCurSel(pMain->m_ListClient.GetCount() - 1);
		}
		else {
			strTmp.Format(_T("[%s:%d] : %s"), strIPAdress, uPortNumber, szBuffer);
			pMain->m_List.AddString(strTime + strTmp);
			pMain->m_List.SetCurSel(pMain->m_List.GetCount() - 1);

			CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
			pServerSocket->SendChatDataAll(szBuffer);
		}
	}

	CSocket::OnReceive(nErrorCode);
}
