#pragma once

#include "Base.h"

_BEGIN(Engine)

class CDXFont final : public CBase
{
private:
	explicit CDXFont(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CDXFont() = default;
public:
	HRESULT Ready_Font(_uint& iWeight, _tchar* pFaceName);
	HRESULT Render_Font(const _tchar* pString, D3DXCOLOR Color, const _matrix* pMatrix);
private:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;
	LPD3DXSPRITE			m_pSprite = nullptr;
	LPD3DXFONT				m_pFont = nullptr;
public:
	static CDXFont* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iWeight, _tchar* pFaceName);
protected:
	virtual void Free();


};

_END