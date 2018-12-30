#pragma once

#include "Base.h"

_BEGIN(Engine)

class CTarget;
class _ENGINE_DLL CTarget_Manager final : public CBase
{
	_DECLARE_SINGLETON(CTarget_Manager)
private:
	explicit CTarget_Manager();
	virtual ~CTarget_Manager() = default;

public:
	HRESULT Add_Target(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pTargetTag, const _uint& iWidth, const _uint& iHeight, D3DFORMAT eFormat, D3DXCOLOR ClearColor);
	HRESULT Add_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag);
	HRESULT Begin_MRT(const _tchar* pMRTTag);
	HRESULT End_MRT(const _tchar* pMRTTag);
	HRESULT SetUp_OnShader(LPD3DXEFFECT pEffect, const char* pConstantName, const _tchar* pTargetTag);
	CTarget* Find_Target(const _tchar* pTargetTag);
	HRESULT Ready_DebugBuffer(const _tchar* pTargetTag, const _float& fStartX, const _float& fStartY, const _float& fSizeX, const _float& fSizeY);
	void Render_DebugBuffer(const _tchar* pMRTTag);
private:
	map<const _tchar*, CTarget*>				m_mapTarget;
	typedef map<const _tchar*, CTarget*>		MAPTARGET;
private:
	map<const _tchar*, list<CTarget*>>			m_mapMRT;
	typedef map<const _tchar*, list<CTarget*>>	MAPMRT;
private:
	list<CTarget*>* Find_MRT(const _tchar* pMRTTag);
protected:
	virtual void Free();

};


_END