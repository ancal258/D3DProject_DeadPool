#include "..\Headers\Font_Manager.h"
#include "DXFont.h"

_IMPLEMENT_SINGLETON(CFont_Manager)

CFont_Manager::CFont_Manager()
{
}

HRESULT CFont_Manager::Add_Font(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar * pFontTag, _uint iWeight, _tchar * pFaceName)
{
	CDXFont*			pFont = Find_Font(pFontTag);

	if (nullptr != pFont)
		return E_FAIL;

	pFont = CDXFont::Create(pGraphic_Device, iWeight, pFaceName);
	if (nullptr == pFont)
		return E_FAIL;

	m_mapFont.insert(MAPFONT::value_type(pFontTag, pFont));

	return NOERROR;
}

void CFont_Manager::Render_Font(const _tchar * pFontTag, const _tchar * pString, D3DXCOLOR Color, const _matrix * pMatrix)
{
	CDXFont*	pFont = Find_Font(pFontTag);

	if (nullptr == pFont)
		return;

	pFont->Render_Font(pString, Color, pMatrix);
}

CDXFont * CFont_Manager::Find_Font(const _tchar * pFontTag)
{
	auto iter = find_if(m_mapFont.begin(), m_mapFont.end(), CFinder_Tag(pFontTag));

	if (iter == m_mapFont.end())
		return nullptr;

	return iter->second;
}

void CFont_Manager::Free()
{
	for (auto& Pair : m_mapFont)
	{
		Safe_Release(Pair.second);
	}
	m_mapFont.clear();
}
