#pragma once

#include "Base.h"

_BEGIN(Engine)

class CLight final : public CBase
{
private:
	explicit CLight(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLight() = default;
public:
	const D3DLIGHT9* Get_LightInfo() const {
		return &m_LightInfo; }
public:
	HRESULT Ready_Light(const D3DLIGHT9* pLightInfo);
private:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;
	D3DLIGHT9				m_LightInfo;
public:
	static CLight* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const D3DLIGHT9* pLightInfo);
protected:
	virtual void Free();
};

_END

