#pragma once

// 렌더타겟 하나를 가지고 있는 객체. == 텍스쳐정보를 가지고 있다.

#include "Base.h"

_BEGIN(Engine)

class CTarget : public CBase
{
private:
	explicit CTarget(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CTarget() = default;
public:
	HRESULT Ready_Target(const _uint& iWidth, const _uint& iHeight, D3DFORMAT eFormat, D3DXCOLOR ClearColor);
	HRESULT SetUp_OnGraphicDev(const _uint& iIndex);
	HRESULT SetUp_OnShader(LPD3DXEFFECT pEffect, const char* pConstantName);
	HRESULT Release_OnGraphicDev(const _uint& iIndex);
	HRESULT Clear_Target();

#ifdef _DEBUG
	HRESULT Ready_DebugBuffer(const _float& fStartX, const _float& fStartY, const _float& fSizeX, const _float& fSizeY);
	void Render_DebugBuffer();
#endif
	LPDIRECT3DDEVICE9			m_pGraphic_Device = nullptr;
	LPDIRECT3DTEXTURE9			m_pTarget_Texture = nullptr;
	LPDIRECT3DSURFACE9			m_pTarget_Surface = nullptr;
	LPDIRECT3DSURFACE9			m_pOld_Surface = nullptr;
	D3DXCOLOR					m_ClearColor;
#ifdef _DEBUG
	LPDIRECT3DVERTEXBUFFER9		m_pVB = nullptr;
	LPDIRECT3DINDEXBUFFER9		m_pIB = nullptr;
#endif

public:
	static CTarget* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _uint& iWidth, const _uint& iHeight, D3DFORMAT eFormat, D3DXCOLOR ClearColor);
protected:
	virtual void Free();
};

_END