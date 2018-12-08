#pragma once

#include "Base.h"



_BEGIN(Engine)

class CDXFont;
class _ENGINE_DLL CFont_Manager final : public CBase
{
	_DECLARE_SINGLETON(CFont_Manager)
private:
	explicit CFont_Manager();
	virtual ~CFont_Manager() = default;
public:
	HRESULT Add_Font(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pFontTag, _uint iWeight, _tchar* pFaceName);
	void Render_Font(const _tchar* pFontTag, const _tchar* pString, D3DXCOLOR Color, const _matrix * pMatrix);
private:
	map<const _tchar*, CDXFont*>			m_mapFont;
	typedef map<const _tchar*, CDXFont*>	MAPFONT;
private:
	CDXFont* Find_Font(const _tchar* pFontTag);

protected:
	virtual void Free();

};

_END