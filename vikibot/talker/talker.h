
#include <vector>
#include <memory>

#include "ThreadSafer.h"

class Talker
{
public:
	int Speak(const CString text);
	void WaitTalkerToFinish();
	Talker();
	~Talker();
private:
	gds::CTextSpeaker m_TextSpeaker;
	std::vector<CString> m_vecTextToSpeak;
	HANDLE m_hThreadTalker;
	HANDLE m_hThreadTalkerFinished;
	bool m_bExitThreadTalker;
	static UINT TalkerThreadProc(LPVOID lparam);
	
};




