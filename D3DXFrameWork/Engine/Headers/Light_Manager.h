#pragma once

#include "Base.h"

_BEGIN(Engine)

class CLight;
class _ENGINE_DLL CLight_Manager final : public CBase
{
	_DECLARE_SINGLETON(CLight_Manager)
private:
	explicit CLight_Manager();
	virtual ~CLight_Manager() = default;
public:
	const D3DLIGHT9* Get_LightInfo(const _uint& iIndex) const;
public:
	HRESULT Add_Light(LPDIRECT3DDEVICE9 pGraphic_Device, const D3DLIGHT9* pLightInfo);
	void Render_Light(LPD3DXEFFECT pEffect);
private:
	list<CLight*>				m_LightList;
	typedef list<CLight*>		LIGHTLIST;
protected:
	virtual void Free();

};

_END