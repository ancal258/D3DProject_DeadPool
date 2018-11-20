#pragma once

#include "Component.h"

_BEGIN(Engine)

class _ENGINE_DLL CTexture final : public CComponent
{
public:
	enum TEXTURETYPE {TYPE_GENERAL, TYPE_CUBE, TYPE_END};
private:
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture() = default;
public:
	HRESULT Ready_Texture(TEXTURETYPE eType, const _tchar* pFileName, const _uint& iCnt);
	HRESULT SetUp_OnShader(LPD3DXEFFECT pEffect, const char* pConstantName, const _uint& iIndex = 0);
private:
	vector<IDirect3DBaseTexture9*>			m_vecTexture;
	typedef vector<IDirect3DBaseTexture9*>	VECTEXTURE;
private:
	_tchar									m_szFileName[MAX_PATH] = L"";
public:
	static CTexture* Create(LPDIRECT3DDEVICE9 pGraphic_Device, TEXTURETYPE eType, const _tchar* pFileName, const _uint& iCnt = 1);
	virtual CComponent* Clone_Component();
protected:
	virtual void Free();
};

_END