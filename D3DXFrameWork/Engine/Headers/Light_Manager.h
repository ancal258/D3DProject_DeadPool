#pragma once

#include "Base.h"

_BEGIN(Engine)

class CLight;
class _ENGINE_DLL CLight_Manager final : public CBase
{
	_DECLARE_SINGLETON(CLight_Manager)
public:
	enum VALUE { STATE_POSITION , STATE_RANGE, STATE_DIFFUSE };
private:
	explicit CLight_Manager();
	virtual ~CLight_Manager() = default;
public:
	const D3DLIGHT9* Get_LightInfo(const _uint& iIndex) const;
	void Set_PointInfo(VALUE eValue, const _vec3* vPosition, const _uint& iIndex);
public:
	HRESULT Add_Light(LPDIRECT3DDEVICE9 pGraphic_Device, const D3DLIGHT9* pLightInfo);
	HRESULT Clear_Light();
	HRESULT Delete_Light(_uint iIndex);
	void Render_Light(LPD3DXEFFECT pEffect);
private:
	list<CLight*>				m_LightList;
	typedef list<CLight*>		LIGHTLIST;
protected:
	virtual void Free();

};

_END