#pragma once
#include <afxmt.h>
#include <vector>

template<class _Ty,class _Ax = std::allocator<_Ty>>  class safevector : private  std::vector<_Ty,_Ax>
{
private:
	CMutex m_mutex;
	
public:
	void safe_push_back(_Ty _Val)
	{	
		m_mutex.Lock();
		push_back(_Val);
		m_mutex.Unlock();
	}
};
