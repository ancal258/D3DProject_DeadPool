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

	void RegistCallbackFunc(function<void(void)> callbackFunc) {
		m_callbackFunc = callbackFunc;	}
	void RegistCallbackCheckPair(function<_bool(_uint, _uint)> callbackCheckPair) {
		m_callbackCheckPair = callbackCheckPair;	}
	void RegistCallbackCheckComboPair(function<_bool(_uint)> callbackCheckComboPair) {
		m_callbackCheckComboPair = callbackCheckComboPair;	}
	void RegistCallbackCheckComboTime(function<_uint(_uint)> callbackCheckComboTime) {
		m_callbackCheckComboTime = callbackCheckComboTime;	}
private:
	LPD3DXANIMATIONCONTROLLER		m_pAniCtrl = nullptr;
	_uint							m_iCurrentTrack = 0;
	_uint							m_iNewTrack = 1;
	_float							m_fTimeAcc = 0.f; // Acc : ´©Àû
	vector<LPD3DXANIMATIONSET>		m_vecAnimSet;
	LPD3DXANIMATIONSET				m_pCurrentAnimSet = nullptr;

	D3DXTRACK_DESC					m_TrackDesc;
	DOUBLE							m_dlPeriod = 0;
	DOUBLE							m_dlCurrentPeriod = 0;
	function<void(void)>			m_callbackFunc = nullptr;
	function<_bool(_uint, _uint)>	m_callbackCheckPair = nullptr;
	function<_bool(_uint)>			m_callbackCheckComboPair = nullptr;
	function<_uint(_uint)>			m_callbackCheckComboTime = nullptr;

	_uint							m_iOldAniIdx = 0;
	_uint							m_iCurrentAniIdx = 0;
public:
	static CAnimationCtrl* Create(LPD3DXANIMATIONCONTROLLER pAniCtrl);
	static CAnimationCtrl* Create(const CAnimationCtrl &rhs);
protected:
	virtual void Free();
};

_END