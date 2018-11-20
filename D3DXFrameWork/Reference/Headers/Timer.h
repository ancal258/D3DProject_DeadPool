#pragma once

#include "Base.h"

// �ð������� ���� Ŭ������.
// m_fTimeDelta�� �����ϰ� �ִ�.
// m_fTimeDelta : �� �������� ������������ �Ÿ��� �ð�.

_BEGIN(Engine)

class CTimer final : public CBase
{
private:
	explicit CTimer();
	virtual ~CTimer() = default;
public:
	_float Get_TimeDelta() const {
		return m_fTimeDelta; } 
public:
	HRESULT Ready_Timer();
	_float Compute_TimeDelta();
private:
	_float						m_fTimeDelta = 0.f;
private:
	LARGE_INTEGER				m_FrameTime;
	LARGE_INTEGER				m_FixTime;
	LARGE_INTEGER				m_LastTime;
	LARGE_INTEGER				m_CpuTick;
public:
	static CTimer* Create();
protected:
	virtual void Free();
};

_END