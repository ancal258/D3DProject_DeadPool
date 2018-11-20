#pragma once

#include "Base.h"
#include "Engine_Defines.h"

class CTexture_Bmp : public CBase
{
private:
	explicit  CTexture_Bmp(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CTexture_Bmp() = default;
public:
	HRESULT Ready_Texture(const _tchar* pFilePath, const _uint& iCnt);
	HRESULT SetUp_OnControl(CStatic* pControl, const _uint& iIndex = 0);
private:
	vector<HBITMAP>			m_vecTextures;
	typedef vector<HBITMAP>	VECTEXTURES;
public:
	static CTexture_Bmp* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pFilePath, const _uint& iCnt = 1);
protected:
	virtual void Free();
};

