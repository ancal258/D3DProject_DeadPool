#pragma once

#include "Base.h"

_BEGIN(Engine)

class CLight final : public CBase
{
private:
	enum VALUE { STATE_POSITION, STATE_RANGE, STATE_DIFFUSE };
private:
	explicit CLight(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLight() = default;
public:
	const D3DLIGHT9* Get_LightInfo() const {
		return &m_LightInfo; }
	void Set_PointInfo(_uint eValue, const _vec3* vPosition);
public:
	HRESULT Ready_Light(const D3DLIGHT9* pLightInfo);
	void Render_Light(LPD3DXEFFECT pEffect);
private:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;
	D3DLIGHT9				m_LightInfo;
	LPDIRECT3DVERTEXBUFFER9	m_pVB = nullptr;
	LPDIRECT3DINDEXBUFFER9	m_pIB = nullptr;
public:
	static CLight* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const D3DLIGHT9* pLightInfo);
protected:
	virtual void Free();
};

_END

