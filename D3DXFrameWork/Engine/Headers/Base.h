#pragma once

#include "Engine_Defines.h"

_BEGIN(Engine)

class _ENGINE_DLL CBase abstract
{
protected:
	explicit CBase();
	virtual ~CBase() = default;
public:
	_ulong AddRef();
	_ulong Release();
protected:
	_ulong			m_dwRefCnt = 0;
protected:
	virtual void Free() = 0;
};

_END