#include "..\Headers\DXFont.h"

CDXFont::CDXFont(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	m_pGraphic_Device->AddRef();
}

HRESULT CDXFont::Ready_Font(_uint& iWeight, _tchar* pFaceName)
{
	D3DXFONT_DESC			FontDesc;
	ZeroMemory(&FontDesc, sizeof(D3DXFONT_DESC));

	FontDesc.Height = 32;
	FontDesc.Width = 16;
	FontDesc.CharSet = DEFAULT_CHARSET;

	FontDesc.Weight = iWeight;
	lstrcpy(FontDesc.FaceName, pFaceName);

	if (FAILED(D3DXCreateFontIndirect(m_pGraphic_Device, &FontDesc, &m_pFont)))
		return E_FAIL;

	if (FAILED(D3DXCreateSprite(m_pGraphic_Device, &m_pSprite)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CDXFont::Render_Font(const _tchar* pString, D3DXCOLOR Color,const _matrix* pMatrix)
{
	if (nullptr == m_pFont)
		return E_FAIL;

	RECT		rcRect = { 100 };

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	
	m_pSprite->SetTransform(pMatrix);

	m_pFont->DrawTextW(m_pSprite, pString, lstrlen(pString), &rcRect, DT_NOCLIP, Color);

	m_pSprite->End();

	return NOERROR;
}

CDXFont * CDXFont::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iWeight, _tchar* pFaceName)
{
	CDXFont*		pInstance = new CDXFont(pGraphic_Device);

	if (FAILED(pInstance->Ready_Font(iWeight, pFaceName)))
	{
		_MSG_BOX("CDXFont Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDXFont::Free()
{
	Safe_Release(m_pSprite);
	Safe_Release(m_pFont);
	Safe_Release(m_pGraphic_Device);
}
