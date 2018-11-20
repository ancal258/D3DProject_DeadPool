#pragma once

#include "Base.h"

// 1초에 정해진 횟수만큼만 true를 리턴하기위한 기능을 가지는 클래스.

_BEGIN(Engine)

class CFrame final : public CBase
{
private:
	explicit CFrame();
	virtual ~CFrame() = default;
public:
	HRESULT Ready_Frame(const _uint& iCallCntPerSec);
	// 1초에 정해진 횟수만큼만 true를 리턴하기위한 기능
	_bool Permit_Call(const _float& fTimeDelta);
private:
	_float			m_fCallTime = 0.f;
	_float			m_fTimeDelta_Acc = 0.f;
public:
	static CFrame* Create(const _uint& iCallCntPerSec);
protected:
	virtual void Free();
};

_END