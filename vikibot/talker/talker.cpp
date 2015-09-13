#include <stdafx.h>

Talker::Talker()
{
	m_bExitThreadTalker = false;
	m_hThreadTalker = CreateEvent(NULL,true,false,L"ThreadTalker");
	m_hThreadTalkerFinished = CreateEvent(NULL,true,false,L"ThreadTalkerFinished");
	AfxBeginThread(TalkerThreadProc,this,THREAD_BASE_PRIORITY_MAX);
	//tsSafeObj.curobj->push_back(L"cool");
}
Talker::~Talker()
{
	m_bExitThreadTalker = true;
	SetEvent(m_hThreadTalker);
	Sleep(100);
	if (m_hThreadTalker != NULL)
	{
		CloseHandle(m_hThreadTalker);
	}
	if(m_hThreadTalkerFinished != NULL)
	{
		CloseHandle(m_hThreadTalkerFinished);
	}
}
int Talker::Speak(const CString text)
{
	m_vecTextToSpeak.push_back(text);
	SetEvent(m_hThreadTalker);
	ResetEvent(m_hThreadTalkerFinished);
	return true;
}
void Talker::WaitTalkerToFinish()
{
	WaitForSingleObject(m_hThreadTalkerFinished,INFINITE);
}
UINT Talker::TalkerThreadProc(LPVOID lparam)
{
	Talker *pObj = (Talker*)lparam;
	CoInitialize(NULL);
	while(WaitForSingleObject(pObj->m_hThreadTalker,INFINITE) == WAIT_OBJECT_0)
	{
		if (pObj->m_bExitThreadTalker == true) break;
		if(pObj->m_vecTextToSpeak.empty())
		{
			ResetEvent(pObj->m_hThreadTalker);
			SetEvent(pObj->m_hThreadTalkerFinished);
		}
		else
		{
			pObj->m_TextSpeaker.Speak(pObj->m_vecTextToSpeak[0]);
			pObj->m_vecTextToSpeak.erase(pObj->m_vecTextToSpeak.begin());
		}
	}
	CoUninitialize();
	return true;
}