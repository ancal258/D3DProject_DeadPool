#pragma once

#include "Base.h"


class CSubtitle_Manager final : public CBase
{
	_DECLARE_SINGLETON(CSubtitle_Manager)
private:
	explicit CSubtitle_Manager();
	virtual ~CSubtitle_Manager() = default;

public:
	void Ready_Subtitle();
	const _tchar* Get_Subtitle();

private:
	vector<const _tchar*>		m_vecSubtitle;
	_uint						m_iCurrentIndex = 0;
protected:
	virtual void Free();
};