#pragma once

#include "Base.h"

_BEGIN(Engine)

class CAnimationCtrl : public CBase
{
private:
	explicit CAnimationCtrl(LPD3DXANIMATIONCONTROLLER pAniCtrl);
	explicit CAnimationCtrl(const CAnimationCtrl &rhs);
	virtual ~CAnimationCtrl() = default;
public:
	HRESULT Ready_AnimationCtrl();
	HRESULT Set_AnimationSet(const _uint& iIndex);
	_bool Is_Finish();
	void Play_AnimationSet(const _float& fTimeDelta);
	void Set_TrackPosition(DOUBLE dlPosition);
	DOUBLE Get_TrackPeriod() {
		return m_dlPeriod;	}
private:
	LPD3DXANIMATIONCONTROLLER		m_pAniCtrl = nullptr;
	_uint							m_iOldAniIdx = 0;
	_uint							m_iCurrentTrack = 0;
	_uint							m_iNewTrack = 1;
	_float							m_fTimeAcc = 0.f; // Acc : ´©Àû


	D3DXTRACK_DESC					m_TrackDesc;
	DOUBLE							m_dlPeriod = 0;
	DOUBLE							m_dlCurrentPeriod = 0;
public:
	static CAnimationCtrl* Create(LPD3DXANIMATIONCONTROLLER pAniCtrl);
	static CAnimationCtrl* Create(const CAnimationCtrl &rhs);
protected:
	virtual void Free();
};

_END