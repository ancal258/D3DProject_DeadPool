#include "..\Headers\Light.h"

CLight::CLight(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	m_pGraphic_Device->AddRef();
}

HRESULT CLight::Ready_Light(const D3DLIGHT9 * pLightInfo)
{
	m_LightInfo = *pLightInfo;

	return NOERROR;
}

CLight * CLight::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const D3DLIGHT9 * pLightInfo)
{
	CLight*		pInstance = new CLight(pGraphic_Device);

	if (FAILED(pInstance->Ready_Light(pLightInfo)))
	{
		_MSG_BOX("CLight Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLight::Free()
{
	Safe_Release(m_pGraphic_Device);
}
